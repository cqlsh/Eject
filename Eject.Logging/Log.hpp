// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <span>

#include "Eject.Logging/LogLevel.hpp"
#include "Eject.Logging/LogSink.hpp"
#include "Eject.Logging/LogSource.hpp"
#include "Eject.Text/Formatting/TextBuilder.hpp"

namespace Eject::Logging
{
    class Log
    {
    public:
        static constexpr std::size_t MessageCapacity = 480;

        template <Eject::Text::Formatting::Appendable... TParts>
            requires(sizeof...(TParts) > 0)
        static void Write(LogLevel level, LogSource source, const TParts&... parts)
        {
            if (static_cast<std::uint8_t>(level) >= m_lowestLevel.load(std::memory_order_relaxed))
            {
                Enqueue(level, source, parts...);
            }
        }

        static void AddSink(const LogSink& sink);
        static void RemoveSink(const LogSink& sink);

        static void Drain()
        {
            if (m_tail.load(std::memory_order_relaxed) != m_head.load(std::memory_order_acquire)) [[unlikely]]
            {
                DeliverQueued();
            }

            if (m_droppedCount.load(std::memory_order_relaxed) != 0) [[unlikely]]
            {
                ReportDropped();
            }
        }

    private:
        struct Reservation
        {
            std::span<char> text;
            std::size_t index = 0;
        };

        template <typename... TParts>
        static void Enqueue(LogLevel level, LogSource source, const TParts&... parts)
        {
            const Reservation reservation = Reserve();
            if (reservation.text.empty())
            {
                return;
            }

            Eject::Text::Formatting::TextBuilder builder(reservation.text);
            (builder.Append(parts), ...);
            Commit(reservation, builder.View().size(), level, source);
        }

        static Reservation Reserve();
        static void Commit(const Reservation& reservation, std::size_t length, LogLevel level, LogSource source);
        static void DeliverQueued();
        static void ReportDropped();

        static std::atomic<std::uint8_t> m_lowestLevel;
        static std::atomic<std::size_t> m_head;
        static std::atomic<std::size_t> m_tail;
        static std::atomic<std::uint32_t> m_droppedCount;
    };
}