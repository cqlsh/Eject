// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#include "Eject.Platform/Windows/Unicode/WideText.hpp"

#include <cstddef>
#include <string>
#include <string_view>

#include <Windows.h>

namespace Eject::Platform::Windows::Unicode
{
    namespace
    {
        constexpr std::size_t LongestUtf8LengthPerWideCharacter = 3;
    }

    std::wstring WideText::FromUtf8(std::string_view text)
    {
        std::wstring wide(text.size(), L'\0');
        const int length = MultiByteToWideChar(
            CP_UTF8,
            MB_ERR_INVALID_CHARS,
            text.data(),
            static_cast<int>(text.size()),
            wide.data(),
            static_cast<int>(wide.size()));
        wide.resize(static_cast<std::size_t>(length));
        return wide;
    }

    std::string WideText::ToUtf8(std::wstring_view text)
    {
        std::string narrow(text.size() * LongestUtf8LengthPerWideCharacter, '\0');
        const int length = WideCharToMultiByte(
            CP_UTF8,
            WC_ERR_INVALID_CHARS,
            text.data(),
            static_cast<int>(text.size()),
            narrow.data(),
            static_cast<int>(narrow.size()),
            nullptr,
            nullptr);
        narrow.resize(static_cast<std::size_t>(length));
        return narrow;
    }
}