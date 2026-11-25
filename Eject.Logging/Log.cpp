// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#include "Eject.Logging/Log.hpp"

#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

#include "Eject.Logging/LogEntry.hpp"
#include "Eject.Logging/LogLevel.hpp"
#include "Eject.Logging/LogSink.hpp"
#include "Eject.Logging/LogSource.hpp"
#include "Eject.Text/Formatting/TextBuilder.hpp"

namespace Eject::Logging
{
    namespace
    {
        constexpr std::size_t SlotCount = 256;
        constexpr std::size_t MaximumSinkCount = 4;
        constexpr std::uint8_t NoSinkLevel = 0xFF;
        constexpr LogSource Source{.name = "Logging"};

        struct Slot
        {
            std::atomic<bool> isReady{false};
            LogLevel level = LogLevel::Debug;
            std::uint16_t length = 0;
            LogSource source;
            std::uint64_t nanoseconds = 0;
            std::array<char, Log::MessageCapacity> text{};
        };

        static_assert(sizeof(Slot) == 512);

        const std::chrono::steady_clock::time_point StartTime = std::chrono::steady_clock::now();
        std::array<Slot, SlotCount> slots{};
        std::array<LogSink, MaximumSinkCount> sinks{};
        std::size_t sinkCount = 0;

        std::uint64_t NanosecondsSinceStart()
        {
            const auto elapsed = std::chrono::steady_clock::now() - StartTime;
            return static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count());
        }

        void Deliver(const LogEntry& entry)
        {
            for (const LogSink& sink : std::span(sinks).first(sinkCount))
            {
                if (entry.level >= sink.minimumLevel)
                {
                    sink.write(sink.context, entry);
                }
            }
        }
    }

    std::atomic<std::uint8_t> Log::m_lowestLevel{NoSinkLevel};
    std::atomic<std::size_t> Log::m_head{0};
    std::atomic<std::size_t> Log::m_tail{0};
    std::atomic<std::uint32_t> Log::m_droppedCount{0};

    void Log::AddSink(const LogSink& sink)
    {
        if (sinkCount == sinks.size())
        {
            return;
        }

        sinks[sinkCount] = sink;
        ++sinkCount;
        const std::uint8_t lowestLevel = m_lowestLevel.load(std::memory_order_relaxed);
        m_lowestLevel.store(
            std::min(lowestLevel, static_cast<std::uint8_t>(sink.minimumLevel)),
            std::memory_order_relaxed);
    }

    void Log::RemoveSink(const LogSink& sink)
    {
        const auto removed = std::ranges::remove_if(
            std::span(sinks).first(sinkCount),
            [&sink](const LogSink& candidate)
            { return candidate.write == sink.write && candidate.context == sink.context; });
        sinkCount -= removed.size();
        std::uint8_t lowestLevel = NoSinkLevel;
        for (const LogSink& remaining : std::span(sinks).first(sinkCount))
        {
            lowestLevel = std::min(lowestLevel, static_cast<std::uint8_t>(remaining.minimumLevel));
        }

        m_lowestLevel.store(lowestLevel, std::memory_order_relaxed);
    }

    Log::Reservation Log::Reserve()
    {
        std::size_t head = m_head.load(std::memory_order_relaxed);
        do
        {
            if (m_tail.load(std::memory_order_acquire) + SlotCount <= head)
            {
                m_droppedCount.fetch_add(1, std::memory_order_relaxed);
                return Reservation{};
            }
        } while (!m_head.compare_exchange_weak(head, head + 1, std::memory_order_acq_rel, std::memory_order_relaxed));

        return Reservation{.text = slots[head % SlotCount].text, .index = head};
    }

    void Log::Commit(const Reservation& reservation, std::size_t length, LogLevel level, LogSource source)
    {
        Slot& slot = slots[reservation.index % SlotCount];
        slot.level = level;
        slot.length = static_cast<std::uint16_t>(length);
        slot.source = source;
        slot.nanoseconds = NanosecondsSinceStart();
        slot.isReady.store(true, std::memory_order_release);
    }

    void Log::DeliverQueued()
    {
        const std::size_t end = m_head.load(std::memory_order_acquire);
        std::size_t tail = m_tail.load(std::memory_order_relaxed);
        while (tail != end)
        {
            Slot& slot = slots[tail % SlotCount];
            if (!slot.isReady.load(std::memory_order_acquire))
            {
                break;
            }

            Deliver(
                LogEntry{
                    .level = slot.level,
                    .source = slot.source,
                    .nanoseconds = slot.nanoseconds,
                    .message = std::string_view(slot.text.data(), slot.length)});
            slot.isReady.store(false, std::memory_order_relaxed);
            ++tail;
            m_tail.store(tail, std::memory_order_release);
        }
    }

    void Log::ReportDropped()
    {
        std::array<char, 64> text{};
        Eject::Text::Formatting::TextBuilder builder(text);
        builder.Append("Log messages dropped because the queue was full: ");
        builder.Append(m_droppedCount.exchange(0, std::memory_order_relaxed));
        Deliver(
            LogEntry{
                .level = LogLevel::Warning,
                .source = Source,
                .nanoseconds = NanosecondsSinceStart(),
                .message = builder.View()});
    }
}