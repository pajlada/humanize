#include "humanize/numbers.hpp"
#include "humanize/time.hpp"

#define CATCH_CONFIG_MAIN
#include "libs/catch.hpp"

#include <inttypes.h>
#include <chrono>
#include <iostream>

TEST_CASE("relativeTime", "xd")
{
    using namespace std::chrono_literals;

    auto systemNow = std::chrono::system_clock::now();
    auto twelveHours = std::chrono::hours(12);
    auto d1 = std::chrono::minutes(1);
    auto d2 = std::chrono::minutes(2);
    auto d3 = std::chrono::minutes(3);
    auto d4 = std::chrono::minutes(750);
    auto d5 = std::chrono::hours(12) + std::chrono::minutes(30);
    auto steadyNow = std::chrono::steady_clock::now();

    humanize::TimeResult rNow = {"now", 0, humanize::TimeResult::Now};

    // make sure simple shit works with both system_clock and steady_clock
    REQUIRE(humanize::relativeTime(systemNow, systemNow) == rNow);
    REQUIRE(humanize::relativeTime(steadyNow, steadyNow) == rNow);

    REQUIRE(humanize::relativeTime(systemNow) == rNow);
    REQUIRE(humanize::relativeTime(steadyNow) == rNow);

    REQUIRE(humanize::relativeTime(twelveHours).str == "12 hours");

    REQUIRE(humanize::relativeTime(d1).str == "1 minute");
    REQUIRE(humanize::relativeTime(d2).str == "2 minutes");
    REQUIRE(humanize::relativeTime(d3).str == "3 minutes");
    REQUIRE(humanize::relativeTime(d4).str == "12 hours, and 30 minutes");
    REQUIRE(humanize::relativeTime(d5).str == "12 hours, and 30 minutes");

    REQUIRE(humanize::relativeTime(systemNow - twelveHours).str == "12 hours");
    REQUIRE(humanize::relativeTime(systemNow - twelveHours, systemNow,
                                   humanize::SuffixType::Short)
                .str == "12h");
}

TEST_CASE("diffTime", "xd")
{
    using namespace std::chrono_literals;

    auto systemNow = std::chrono::system_clock::now();
    auto twelveHours = std::chrono::hours(12);
    auto d1 = std::chrono::minutes(1);
    auto d2 = std::chrono::minutes(2);
    auto d3 = std::chrono::minutes(3);
    auto d4 = std::chrono::minutes(750);
    auto d5 = std::chrono::hours(12) + std::chrono::minutes(30);
    auto steadyNow = std::chrono::steady_clock::now();

    humanize::TimeResult rNow = {"now", 0, humanize::TimeResult::Now};

    // make sure simple shit works with both system_clock and steady_clock
    REQUIRE(humanize::diffTime(systemNow, systemNow) == "now");
    REQUIRE(humanize::diffTime(steadyNow, steadyNow) == "now");

    REQUIRE(humanize::diffTime(systemNow) == "now");
    REQUIRE(humanize::diffTime(steadyNow) == "now");

    REQUIRE(humanize::diffTime(twelveHours) == "12 hours");

    REQUIRE(humanize::diffTime(d1) == "1 minute");
    REQUIRE(humanize::diffTime(d2) == "2 minutes");
    REQUIRE(humanize::diffTime(d3) == "3 minutes");
    REQUIRE(humanize::diffTime(d4) == "12 hours, and 30 minutes");
    REQUIRE(humanize::diffTime(d5) == "12 hours, and 30 minutes");

    REQUIRE(humanize::diffTime(systemNow - twelveHours) == "12 hours");
    REQUIRE(humanize::diffTime(systemNow - twelveHours, systemNow,
                               humanize::SuffixType::Short) == "12h");
}

TEST_CASE("compactInteger", "xd")
{
    REQUIRE(humanize::compactInteger(0) == "0");
    REQUIRE(humanize::compactInteger(-0) == "0");

    REQUIRE(humanize::compactInteger(5) == "5");
    REQUIRE(humanize::compactInteger(999) == "999");
    REQUIRE(humanize::compactInteger(-5) == "-5");
    REQUIRE(humanize::compactInteger(-999) == "-999");
    REQUIRE(humanize::compactInteger(static_cast<int64_t>(5)) == "5");
    REQUIRE(humanize::compactInteger(static_cast<int32_t>(5)) == "5");
    REQUIRE(humanize::compactInteger(static_cast<int16_t>(5)) == "5");
    REQUIRE(humanize::compactInteger(static_cast<int8_t>(5)) == "5");
    REQUIRE(humanize::compactInteger(static_cast<int8_t>(-120)) == "-120");
    REQUIRE(humanize::compactInteger(static_cast<int8_t>(120)) == "120");

    REQUIRE(humanize::compactInteger(1000) == "1k");
    REQUIRE(humanize::compactInteger(1499) == "1k");
    REQUIRE(humanize::compactInteger(1500) == "1k");
    REQUIRE(humanize::compactInteger(1500, 1) == "1.5k");
    REQUIRE(humanize::compactInteger(1500, 2) == "1.50k");
    REQUIRE(humanize::compactInteger(1501) == "1k");
    REQUIRE(humanize::compactInteger(1999) == "1k");
    REQUIRE(humanize::compactInteger(2000) == "2k");
    REQUIRE(humanize::compactInteger(20000) == "20k");
    REQUIRE(humanize::compactInteger(29000) == "29k");
    REQUIRE(humanize::compactInteger(29900) == "29k");
    REQUIRE(humanize::compactInteger(200000) == "200k");
    REQUIRE(humanize::compactInteger(290000) == "290k");
    REQUIRE(humanize::compactInteger(299000) == "299k");
    REQUIRE(humanize::compactInteger(299000, 1) == "299.0k");
    REQUIRE(humanize::compactInteger(299900, 1) == "299.9k");
    REQUIRE(humanize::compactInteger(3000000) == "3M");
    REQUIRE(humanize::compactInteger(3000000, 1) == "3.0M");
    REQUIRE(humanize::compactInteger(3100000, 1) == "3.1M");
    REQUIRE(humanize::compactInteger(3190000, 1) == "3.1M");
    REQUIRE(humanize::compactInteger(3900000, 1) == "3.9M");
    REQUIRE(humanize::compactInteger(3990000, 1) == "3.9M");

    REQUIRE(humanize::compactInteger(-1000) == "-1k");
    REQUIRE(humanize::compactInteger(-1499) == "-1k");
    REQUIRE(humanize::compactInteger(-1500) == "-1k");
    REQUIRE(humanize::compactInteger(-1500, 1) == "-1.5k");
    REQUIRE(humanize::compactInteger(-1500, 2) == "-1.50k");
    REQUIRE(humanize::compactInteger(-1501) == "-1k");
    REQUIRE(humanize::compactInteger(-1999) == "-1k");
    REQUIRE(humanize::compactInteger(-2000) == "-2k");
    REQUIRE(humanize::compactInteger(-20000) == "-20k");
    REQUIRE(humanize::compactInteger(-29000) == "-29k");
    REQUIRE(humanize::compactInteger(-29900) == "-29k");
    REQUIRE(humanize::compactInteger(-200000) == "-200k");
    REQUIRE(humanize::compactInteger(-290000) == "-290k");
    REQUIRE(humanize::compactInteger(-299000) == "-299k");
    REQUIRE(humanize::compactInteger(-299000, 1) == "-299.0k");
    REQUIRE(humanize::compactInteger(-299900, 1) == "-299.9k");
    REQUIRE(humanize::compactInteger(-3000000) == "-3M");
    REQUIRE(humanize::compactInteger(-3000000, 1) == "-3.0M");
    REQUIRE(humanize::compactInteger(-3100000, 1) == "-3.1M");
    REQUIRE(humanize::compactInteger(-3190000, 1) == "-3.1M");
    REQUIRE(humanize::compactInteger(-3900000, 1) == "-3.9M");
    REQUIRE(humanize::compactInteger(-3990000, 1) == "-3.9M");

    REQUIRE(humanize::compactInteger(static_cast<uint64_t>(5)) == "5");
    REQUIRE(humanize::compactInteger(static_cast<uint32_t>(5)) == "5");
    REQUIRE(humanize::compactInteger(static_cast<uint16_t>(5)) == "5");
    REQUIRE(humanize::compactInteger(static_cast<uint8_t>(5)) == "5");
    REQUIRE(humanize::compactInteger(static_cast<uint8_t>(120)) == "120");
    REQUIRE(humanize::compactInteger(static_cast<uint8_t>(240)) == "240");

    /*
    REQUIRE(humanize::compactInteger(5.5) == "5.500000");
    REQUIRE(humanize::compactInteger(-5.5) == "-5.500000");
    */
}

TEST_CASE("ordinal", "xd")
{
    REQUIRE(humanize::ordinal(-1) == "-1");
    REQUIRE(humanize::ordinal(0) == "0th");
    REQUIRE(humanize::ordinal(1) == "1st");
    REQUIRE(humanize::ordinal(2) == "2nd");
    REQUIRE(humanize::ordinal(3) == "3rd");
    REQUIRE(humanize::ordinal(4) == "4th");
    REQUIRE(humanize::ordinal(5) == "5th");
    REQUIRE(humanize::ordinal(52) == "52nd");
    REQUIRE(humanize::ordinal(95) == "95th");
    REQUIRE(humanize::ordinal(100) == "100th");
    REQUIRE(humanize::ordinal(101) == "101st");
    REQUIRE(humanize::ordinal(102) == "102nd");
    REQUIRE(humanize::ordinal(103) == "103rd");
    REQUIRE(humanize::ordinal(104) == "104th");
    REQUIRE(humanize::ordinal(195) == "195th");
    REQUIRE(humanize::ordinal(5000) == "5000th");
}

TEST_CASE("toFixed", "xd")
{
    REQUIRE(humanize::toFixed(502.2) == "502");
    REQUIRE(humanize::toFixed(502.29123123) == "502");
    REQUIRE(humanize::toFixed(502.9) == "502");
    REQUIRE(humanize::toFixed(503.2) == "503");
}
