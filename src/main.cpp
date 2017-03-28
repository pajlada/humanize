#include "humanize/numbers.hpp"
#include "humanize/time.hpp"

#define CATCH_CONFIG_MAIN
#include "libs/catch.hpp"

#include <inttypes.h>

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
