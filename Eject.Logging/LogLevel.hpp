// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include <cstdint>

namespace Eject::Logging
{
    enum class LogLevel : std::uint8_t
    {
        Debug,
        Info,
        Warning,
        Error
    };
}