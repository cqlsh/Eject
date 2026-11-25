// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include <concepts>
#include <cstddef>
#include <span>
#include <string_view>
#include <type_traits>

#include "Eject.Text/Formatting/NumberText.hpp"

namespace Eject::Text::Formatting
{
    template <typename TPart>
    concept TextPart = !std::is_null_pointer_v<TPart> && std::convertible_to<const TPart&, std::string_view>;

    template <typename TPart>
    concept Appendable = TextPart<TPart> || Number<TPart>;

    class TextBuilder
    {
    public:
        explicit TextBuilder(std::span<char> buffer)
            : m_begin(buffer.data())
            , m_cursor(buffer.data())
            , m_limit(buffer.data() + buffer.size() - 1)
        {
        }

        template <TextPart TPart>
        void Append(const TPart& part)
        {
            if constexpr (std::is_pointer_v<TPart>)
            {
                if (part == nullptr)
                {
                    return;
                }
            }

            const std::string_view text(part);
            const auto remaining = static_cast<std::size_t>(m_limit - m_cursor);
            if (text.size() <= remaining)
            {
                m_cursor += text.copy(m_cursor, text.size());
                return;
            }

            std::size_t length = remaining;
            while (length > 0 && (static_cast<unsigned char>(text[length]) & 0xC0U) == 0x80U)
            {
                --length;
            }

            m_cursor += text.copy(m_cursor, length);
            m_limit = m_cursor;
        }

        template <Number TPart>
        void Append(TPart part)
        {
            const std::size_t length = NumberText::Write(std::span<char>(m_cursor, m_limit), part);
            if (length == 0)
            {
                m_limit = m_cursor;
                return;
            }

            m_cursor += length;
        }

        std::string_view View() const
        {
            return std::string_view(m_begin, static_cast<std::size_t>(m_cursor - m_begin));
        }

        const char* Terminate()
        {
            *m_cursor = '\0';
            return m_begin;
        }

    private:
        char* m_begin;
        char* m_cursor;
        char* m_limit;
    };
}