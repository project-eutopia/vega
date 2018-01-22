#pragma once

#include "test_helper.h"

#include "vega/string.h"
#include "vega/time.h"
#include "vega/manipulators/padded_string_manipulator.h"

using namespace vega;

TEST(TimeTest, constructor_test) {
  Time time("235959.999999");
  EXPECT_EQ(time.str(), "235959.999999");
}

TEST(TimeTest, range_test) {
  {
    Time time(" - 120000 ");
    EXPECT_TRUE(time.is_range());
    EXPECT_FALSE(time.lower());
    EXPECT_TRUE(time.upper());

    EXPECT_EQ(time.str(), "-120000");
  }

  {
    Time time(" 2330 - ");
    EXPECT_TRUE(time.is_range());
    EXPECT_TRUE(time.lower());
    EXPECT_FALSE(time.upper());

    EXPECT_EQ(time.str(), "2330-");
  }

  {
    Time time("08-17");
    EXPECT_TRUE(time.is_range());
    EXPECT_TRUE(time.lower());
    EXPECT_TRUE(time.upper());

    EXPECT_EQ(time.str(), "08-17");
  }
}

TEST(TimeTest, element_from_string_test) {
  Time time = vega::from_string<Time>("120000");
  EXPECT_EQ(time.str(), "120000");
}

TEST(TimeTest, to_string_test) {
  Time time("112233");
  EXPECT_EQ(vega::to_string(time), std::string("112233"));
}

TEST(TimeTest, manipulator_test) {
  vega::manipulators::PaddedStringManipulator<Time> date_manipulator{};
  date_manipulator.emplace_back(std::string("120000"));
  date_manipulator.emplace_back(std::string("180059"));

  EXPECT_EQ(date_manipulator[0].str(), "120000");
  EXPECT_EQ(date_manipulator[1].str(), "180059");
  EXPECT_EQ(date_manipulator.str(), std::string("120000\\180059 "));
}
