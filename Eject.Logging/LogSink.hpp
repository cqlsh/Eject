// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include "Eject.Logging/LogEntry.hpp"
#include "Eject.Logging/LogLevel.hpp"

namespace Eject::Logging
{
    struct LogSink
    {
        void (*write)(void* context, const LogEntry& entry) = nullptr;
        void* context = nullptr;
        LogLevel minimumLevel = LogLevel::Info;
    };
}