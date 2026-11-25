// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include <string_view>

namespace Eject::Logging
{
    struct LogSource
    {
        std::string_view name;
    };
}