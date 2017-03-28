#pragma once

#include <inttypes.h>

namespace humanize {

template <class T>
class format_string
{
public:
    static constexpr const char *
    format() noexcept
    {
        return "xd";
    }
};

template <>
class format_string<int64_t>
{
public:
    static constexpr const char *
    format() noexcept
    {
        return "%" PRId64;
    }

    static constexpr const char *
    scan_format() noexcept
    {
        return "%" SCNd64;
    }
};

template <>
class format_string<int32_t>
{
public:
    static constexpr const char *
    format() noexcept
    {
        return "%" PRId32;
    }

    static constexpr const char *
    scan_format() noexcept
    {
        return "%" SCNd32;
    }
};

template <>
class format_string<int16_t>
{
public:
    static constexpr const char *
    format() noexcept
    {
        return "%" PRId16;
    }

    static constexpr const char *
    scan_format() noexcept
    {
        return "%" SCNd16;
    }
};

template <>
class format_string<int8_t>
{
public:
    static constexpr const char *
    format() noexcept
    {
        return "%" PRId8;
    }

    static constexpr const char *
    scan_format() noexcept
    {
        return "%" SCNd8;
    }
};

template <>
class format_string<uint64_t>
{
public:
    static constexpr const char *
    format() noexcept
    {
        return "%" PRIu64;
    }

    static constexpr const char *
    scan_format() noexcept
    {
        return "%" SCNu64;
    }
};

template <>
class format_string<uint32_t>
{
public:
    static constexpr const char *
    format() noexcept
    {
        return "%" PRIu32;
    }

    static constexpr const char *
    scan_format() noexcept
    {
        return "%" SCNu32;
    }
};

template <>
class format_string<uint16_t>
{
public:
    static constexpr const char *
    format() noexcept
    {
        return "%" PRIu16;
    }

    static constexpr const char *
    scan_format() noexcept
    {
        return "%" SCNu16;
    }
};

template <>
class format_string<uint8_t>
{
public:
    static constexpr const char *
    format() noexcept
    {
        return "%" PRIu8;
    }

    static constexpr const char *
    scan_format() noexcept
    {
        return "%" SCNu8;
    }
};

}  // namespace humanize
