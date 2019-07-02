#pragma once

#include <time.h>
#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>

namespace humanize {

// things that are implemented in C++17
namespace future {

template <class T>
struct is_duration : public std::false_type {
};

template <class Rep, class Period>
struct is_duration<std::chrono::duration<Rep, Period>> : public std::true_type {
};

template <
    class Rep, class Period,
    class = std::enable_if_t<(std::chrono::duration<Rep, Period>::min() <
                              std::chrono::duration<Rep, Period>::zero())>>
constexpr std::chrono::duration<Rep, Period>
chronoAbs(std::chrono::duration<Rep, Period> d)
{
    return d >= d.zero() ? d : -d;
}

}  // namespace future

enum SuffixType {
    Long,
    Short,
};

namespace {

static const char *longSuffixesSingular[6] = {
    "year",    //
    "month",   //
    "day",     //
    "hour",    //
    "minute",  //
    "second",  //
};

static const char *longSuffixesPlural[6] = {
    "years",    //
    "months",   //
    "days",     //
    "hours",    //
    "minutes",  //
    "seconds",  //
};

static const char *shortSuffixes[6] = {
    "y",  //
    "m",  //
    "d",  //
    "h",  //
    "m",  //
    "s",  //
};

struct TimeDiffData {
    uint16_t count;

    const char *longSuffixSingular;
    const char *longSuffixPlural;
    const char *shortSuffix;

    TimeDiffData(uint16_t _count, int index)
        : count(_count)
        , longSuffixSingular(longSuffixesSingular[index])
        , longSuffixPlural(longSuffixesPlural[index])
        , shortSuffix(shortSuffixes[index])
    {
    }

    const char *
    getSuffix(SuffixType type) const
    {
        switch (type) {
            case SuffixType::Long: {
                if (this->count == 1) {
                    return this->longSuffixSingular;
                }
                return this->longSuffixPlural;
            } break;

            case SuffixType::Short:
            default: {
                return this->shortSuffix;
            } break;
        }
    }
};

constexpr int64_t secondsPerYear = 31536000;
constexpr int64_t secondsPerMonth = 2628000;
constexpr int64_t secondsPerDay = 86400;
constexpr int64_t secondsPerHour = 3600;
constexpr int64_t secondsPerMinute = 60;

constexpr double daysPerMonth = 30.41666666666667;
constexpr int64_t monthsPerYear = 12;
constexpr int64_t hoursPerDay = 24;
constexpr int64_t minutesPerHour = 60;

}  // anonymous namespace

struct TimeResult {
    std::string str;
    int64_t secondsDiff;

    enum {
        Past,
        Now,
        Future,
    } resultType;

    bool
    operator==(const TimeResult &rhs) const
    {
        return this->secondsDiff == rhs.secondsDiff &&
               this->resultType == rhs.resultType;
    }

    bool
    operator!=(const TimeResult &rhs) const
    {
        return !(*this == rhs);
    }
};

template <class Clock, class Duration = typename Clock::duration>
TimeResult
relativeTime(const std::chrono::time_point<Clock, Duration> &start,
             const std::chrono::time_point<Clock, Duration> &end,
             SuffixType suffixType = SuffixType::Long, unsigned maxPoints = 2)
{
    TimeResult tr;

    auto intDiff = future::chronoAbs(
        std::chrono::duration_cast<std::chrono::seconds>(start - end));
    int64_t secondsDiff = intDiff.count();

    tr.secondsDiff = secondsDiff;

    if (secondsDiff == 0) {
        tr.str = "now";
        tr.resultType = TimeResult::Now;
        return tr;
    }

    if (secondsDiff < 0) {
        tr.resultType = TimeResult::Future;
    } else {
        tr.resultType = TimeResult::Past;
    }

    TimeDiffData tdd[] = {
        TimeDiffData(static_cast<uint16_t>(trunc(secondsDiff / secondsPerYear)),
                     0),
        TimeDiffData(static_cast<uint16_t>(
                         trunc(secondsDiff / secondsPerMonth % monthsPerYear)),
                     1),
        TimeDiffData(static_cast<uint16_t>(trunc(
                         fmod(secondsDiff / secondsPerDay, daysPerMonth))),
                     2),
        TimeDiffData(static_cast<uint16_t>(
                         trunc(secondsDiff / secondsPerHour % hoursPerDay)),
                     3),
        TimeDiffData(static_cast<uint16_t>(trunc(
                         secondsDiff / secondsPerMinute % minutesPerHour)),
                     4),
        TimeDiffData(static_cast<uint16_t>(secondsDiff % secondsPerMinute), 5),
    };

    char ret[1024];
    int length = 0;

    unsigned pointsUsed = 0;
    for (const TimeDiffData &cTdd : tdd) {
        if (pointsUsed >= maxPoints) {
            break;
        }

        if (cTdd.count == 0) {
            continue;
        }

        if (pointsUsed > 0) {
            if (suffixType == SuffixType::Long) {
                if (pointsUsed == maxPoints - 1) {
                    strcpy(ret + length, ", and ");
                    length += 6;
                } else {
                    strcpy(ret + length, ", ");
                    length += 2;
                }
            }
        }

        // Long suffixtype means space between count and suffix, short means no
        // space
        if (suffixType == SuffixType::Long) {
            length += std::sprintf(ret + length, "%d %s", cTdd.count,
                                   cTdd.getSuffix(suffixType));
        } else {
            length += std::sprintf(ret + length, "%d%s", cTdd.count,
                                   cTdd.getSuffix(suffixType));
        }
        ++pointsUsed;
    }

    tr.str = ret;

    return tr;
}

template <class Clock>
TimeResult
relativeTime(const std::chrono::time_point<Clock> &end,
             SuffixType suffixType = SuffixType::Long, unsigned maxPoints = 2)
{
    return humanize::relativeTime(Clock::now(), end, suffixType, maxPoints);
}

template <class Duration, class Clock = std::chrono::system_clock,
          typename = typename std::enable_if<
              future::is_duration<Duration>::value>::type>
TimeResult
relativeTime(const Duration &duration, SuffixType suffixType = SuffixType::Long,
             unsigned maxPoints = 2)
{
    auto c1 = std::chrono::time_point_cast<Duration>(Clock::now());
    auto c2 = c1 + duration;
    return humanize::relativeTime(c1, c2, suffixType, maxPoints);
}

template <class Clock, class Duration = typename Clock::duration>
std::string
diffTime(const std::chrono::time_point<Clock, Duration> &start,
         const std::chrono::time_point<Clock, Duration> &end,
         SuffixType suffixType = SuffixType::Long, unsigned maxPoints = 2)
{
    return relativeTime(start, end, suffixType, maxPoints).str;
}

template <class Clock>
std::string
diffTime(const std::chrono::time_point<Clock> &end,
         SuffixType suffixType = SuffixType::Long, unsigned maxPoints = 2)
{
    return humanize::diffTime(Clock::now(), end, suffixType, maxPoints);
}

template <class Duration, class Clock = std::chrono::system_clock,
          typename = typename std::enable_if<
              future::is_duration<Duration>::value>::type>
std::string
diffTime(const Duration &duration, SuffixType suffixType = SuffixType::Long,
         unsigned maxPoints = 2)
{
    auto c1 = std::chrono::time_point_cast<Duration>(Clock::now());
    auto c2 = c1 + duration;
    return humanize::diffTime(c1, c2, suffixType, maxPoints);
}

template <class Clock>
std::string
toIso(const std::chrono::time_point<Clock> &now, tm *threadLocalTmBuf = nullptr)
{
    static tm tmBuf;
    time_t tt = std::chrono::system_clock::to_time_t(now);

    if (threadLocalTmBuf == nullptr) {
        threadLocalTmBuf = &tmBuf;
    }
    auto local_tm = *::localtime_r(&tt, threadLocalTmBuf);

    static const char *fmt = "%Y-%m-%d %H:%M:%S";
    char buf[32];

    strftime(buf, 32, fmt, &local_tm);

    std::string timeString(buf);

    std::stringstream ss;

    std::chrono::time_point<Clock, std::chrono::milliseconds> ms =
        std::chrono::time_point_cast<std::chrono::milliseconds>(now);

    ss << buf << '.' << std::setfill('0') << std::setw(3)
       << (ms.time_since_epoch().count() % 999);

    return ss.str();
}

}  // namespace humanize
