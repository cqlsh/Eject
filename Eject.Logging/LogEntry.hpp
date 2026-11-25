// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include <cstdint>
#include <string_view>

#include "Eject.Logging/LogLevel.hpp"
#include "Eject.Logging/LogSource.hpp"

namespace Eject::Logging
{
    struct LogEntry
    {
        LogLevel level = LogLevel::Info;
        LogSource source;
        std::uint64_t nanoseconds = 0;
        std::string_view message;
    };
}