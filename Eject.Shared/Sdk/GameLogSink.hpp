// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include <array>
#include <string_view>

#include <scssdk.h>

#include "Eject.Logging/Log.hpp"
#include "Eject.Logging/LogEntry.hpp"
#include "Eject.Logging/LogSink.hpp"

namespace Eject::Shared::Sdk
{
    class GameLogSink
    {
    public:
        explicit GameLogSink(scs_log_t log);

        Eject::Logging::LogSink Sink();

    private:
        static constexpr std::string_view Prefix = "[Eject] ";

        static void Write(void* context, const Eject::Logging::LogEntry& entry);

        scs_log_t m_log;
        std::array<char, Prefix.size() + Eject::Logging::Log::MessageCapacity> m_line{};
    };
}