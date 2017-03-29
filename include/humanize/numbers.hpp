#pragma once

#include "humanize/format_string.hpp"

#include <array>
#include <string>
#include <type_traits>

namespace humanize {

namespace {

struct NumberStruct {
    int length;
    char suffix;
};

// return const char * with arguments typename Number, and input char buffer
// with size of numeric_limits<Number, and with input format string from
// format_string<Number>
template <typename Number>
void
numberToChars(char *str, Number x)
{
    std::sprintf(str, format_string<Number>::format(), x);
}

template <typename Number>
void
absValue(Number &x,
         typename std::enable_if<std::is_signed<Number>::value>::type * = 0)
{
    x = static_cast<Number>(std::abs(x));
}
template <typename Number>
void
absValue(Number &,
         typename std::enable_if<std::is_unsigned<Number>::value>::type * = 0)
{
    // do nothing
}

template <bool IsSigned>
struct SignedHelper {
};

template <>
struct SignedHelper<false> {
    template <typename Number>
    static char *
    getStringWithoutSign(char *str, Number)
    {
        return str;
    }

    template <typename Number>
    static void
    test(char *str, Number, unsigned decimals, const char *numberStr,
         int decimalIndex, const NumberStruct &ns)
    {
        if (decimals > 0) {
            std::sprintf(str, "%.*s.%.*s%c", decimalIndex, numberStr, decimals,
                         numberStr + decimalIndex, ns.suffix);
        } else {
            std::sprintf(str, "%.*s%c", decimalIndex, numberStr, ns.suffix);
        }
    }
};

template <>
struct SignedHelper<true> {
    template <typename Number>
    static char *
    getStringWithoutSign(char *str, Number number)
    {
        if (number < 0) {
            return str + 1;
        } else {
            return str;
        }
    }

    template <typename Number>
    static void
    test(char *str, Number number, unsigned decimals, const char *numberStr,
         int decimalIndex, const NumberStruct &ns)
    {
        if (number < 0) {
            if (decimals > 0) {
                std::sprintf(str, "-%.*s.%.*s%c", decimalIndex, numberStr,
                             decimals, numberStr + decimalIndex, ns.suffix);
            } else {
                std::sprintf(str, "-%.*s%c", decimalIndex, numberStr,
                             ns.suffix);
            }
        } else {
            SignedHelper<false>::test(str, number, decimals, numberStr,
                                      decimalIndex, ns);
        }
    }
};

}  // anonymous namespace

// Signed integer
template <typename Number,
          typename = std::enable_if<std::is_integral<Number>::value>::type>
std::string
compactInteger(Number number, unsigned decimals = 0)
{
    constexpr auto maxStringSize = std::numeric_limits<Number>::digits10 +
                                   std::numeric_limits<Number>::is_signed + 2;
    char str[maxStringSize];

    numberToChars<Number>(str, number);
    Number absNumber = number;
    absValue(absNumber);

    static std::array<NumberStruct, 4> numberLengths = {{
        {13, 'T'},  //
        {10, 'B'},  //
        {7, 'M'},   //
        {4, 'k'},   //
    }};

    char *numberStr =
        SignedHelper<std::is_signed<Number>::value>::getStringWithoutSign(
            str, number);

    int absNumberLength = static_cast<int>(strlen(numberStr));

    if (absNumber < 1000) {
        return str;
    }

    // do complicated shit
    for (const auto &numberStruct : numberLengths) {
        if (absNumberLength >= numberStruct.length) {
            // Found matching NumberStruct
            int decimalIndex = absNumberLength - numberStruct.length + 1;

            char finalStr[maxStringSize];

            SignedHelper<std::is_signed<Number>::value>::test(
                finalStr, number, decimals, numberStr, decimalIndex,
                numberStruct);

            return finalStr;
        }
    }

    // This should never happen
    return "?";
}

}  // namespace humanize
