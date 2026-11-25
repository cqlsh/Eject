// Copyright (c) cqlsh. All rights reserved.
// Licensed under the Eject License. See LICENSE in the repository root.

#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <charconv>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <system_error>
#include <type_traits>

namespace Eject::Text::Formatting
{
    template <typename TValue>
    concept Number = std::is_arithmetic_v<TValue> && !std::same_as<TValue, bool> && !std::same_as<TValue, char>;

    class NumberText
    {
    public:
        template <Number TValue>
        static std::size_t Write(std::span<char> target, TValue value)
        {
            if constexpr (std::floating_point<TValue>)
            {
                const std::to_chars_result result = std::to_chars(target.data(), target.data() + target.size(), value);
                return result.ec == std::errc{} ? static_cast<std::size_t>(result.ptr - target.data()) : 0;
            }
            else
            {
                using Magnitude = std::make_unsigned_t<decltype(+value)>;
                auto magnitude = static_cast<Magnitude>(+value);
                std::size_t signLength = 0;
                if constexpr (std::is_signed_v<TValue>)
                {
                    if (value < 0)
                    {
                        magnitude = Magnitude{0} - magnitude;
                        signLength = 1;
                    }
                }

                const Magnitude nonZero = magnitude | 1U;
                const std::size_t estimate =
                    (static_cast<std::size_t>(std::bit_width(nonZero)) * DigitsPerBitScaled) >> DigitsPerBitShift;
                const std::size_t digitCount = estimate + (nonZero >= PowersOfTen[estimate] ? 1U : 0U);
                if (signLength + digitCount > target.size())
                {
                    return 0;
                }

                char* const digits = target.data() + signLength;
                if (signLength != 0)
                {
                    target.front() = '-';
                }

                std::size_t position = digitCount;
                while (position >= 2)
                {
                    const auto pair = static_cast<std::size_t>(magnitude % 100) * 2;
                    magnitude /= 100;
                    position -= 2;
                    std::copy_n(DigitPairs.data() + pair, 2, digits + position);
                }

                if (position == 1)
                {
                    *digits = static_cast<char>('0' + magnitude);
                }

                return signLength + digitCount;
            }
        }

    private:
        static constexpr std::size_t DigitsPerBitScaled = 1233;
        static constexpr std::size_t DigitsPerBitShift = 12;
        static constexpr std::string_view DigitPairs = "00010203040506070809"
                                                       "10111213141516171819"
                                                       "20212223242526272829"
                                                       "30313233343536373839"
                                                       "40414243444546474849"
                                                       "50515253545556575859"
                                                       "60616263646566676869"
                                                       "70717273747576777879"
                                                       "80818283848586878889"
                                                       "90919293949596979899";
        static constexpr std::array<std::uint64_t, 20> PowersOfTen{
            1ULL,
            10ULL,
            100ULL,
            1000ULL,
            10000ULL,
            100000ULL,
            1000000ULL,
            10000000ULL,
            100000000ULL,
            1000000000ULL,
            10000000000ULL,
            100000000000ULL,
            1000000000000ULL,
            10000000000000ULL,
            100000000000000ULL,
            1000000000000000ULL,
            10000000000000000ULL,
            100000000000000000ULL,
            1000000000000000000ULL,
            10000000000000000000ULL};
    };
}