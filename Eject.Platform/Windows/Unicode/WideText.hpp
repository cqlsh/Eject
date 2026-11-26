// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include <string>
#include <string_view>

namespace Eject::Platform::Windows::Unicode
{
    class WideText
    {
    public:
        static std::wstring FromUtf8(std::string_view text);
        static std::string ToUtf8(std::wstring_view text);
    };
}