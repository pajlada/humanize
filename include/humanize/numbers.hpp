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
    x = std::abs(x);
}
template <typename Number>
void
absValue(Number &,
         typename std::enable_if<std::is_unsigned<Number>::value>::type * = 0)
{
    // do nothing
}

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

    char *cNoSign;

    if (std::is_signed<Number>::value && number < 0) {
        cNoSign = str + 1;
    } else {
        cNoSign = str;
    }

    int absNumberLength = static_cast<int>(strlen(cNoSign));

    if (absNumber < 1000) {
        return str;
    }

    // do complicated shit
    NumberStruct numberStruct;
    for (auto p : numberLengths) {
        if (absNumberLength >= p.length) {
            numberStruct = p;
            break;
        }
    }

    int decimalIndex = absNumberLength - numberStruct.length + 1;

    char finalStr[maxStringSize];

    if (std::is_signed<Number>::value && number < 0) {
        if (decimals > 0) {
            std::sprintf(finalStr, "-%.*s.%.*s%c", decimalIndex, cNoSign,
                         decimals, cNoSign + decimalIndex, numberStruct.suffix);
        } else {
            std::sprintf(finalStr, "-%.*s%c", decimalIndex, cNoSign,
                         numberStruct.suffix);
        }
    } else {
        if (decimals > 0) {
            std::sprintf(finalStr, "%.*s.%.*s%c", decimalIndex, cNoSign,
                         decimals, cNoSign + decimalIndex, numberStruct.suffix);
        } else {
            std::sprintf(finalStr, "%.*s%c", decimalIndex, cNoSign,
                         numberStruct.suffix);
        }
    }

    return finalStr;

    char subStr[maxStringSize];
    // substr original str
    memcpy(subStr, cNoSign, decimalIndex);
    subStr[decimalIndex] = numberStruct.suffix;
    subStr[decimalIndex + 1] = '\0';

    return subStr;
}

}  // namespace humanize