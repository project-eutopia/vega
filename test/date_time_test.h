#pragma once

#include "test_helper.h"

#include "vega/string.h"
#include "vega/date_time.h"
#include "vega/manipulators/padded_string_manipulator.h"

using namespace vega;

TEST(DateTimeTest, constructor_test) {
  DateTime dt("20001010120000");
  EXPECT_EQ(dt.str(), "20001010120000");
}

TEST(DateTimeTest, range_test) {
  {
    DateTime dt(" - 22221111000000.1 ");
    EXPECT_TRUE(dt.is_range());
    EXPECT_FALSE(dt.lower());
    EXPECT_TRUE(dt.upper());

    EXPECT_EQ(dt.str(), "-22221111000000.1");
  }

  {
    DateTime dt(" 19990101235960.003366+1200 - ");
    EXPECT_TRUE(dt.is_range());
    EXPECT_TRUE(dt.lower());
    EXPECT_FALSE(dt.upper());

    EXPECT_EQ(dt.str(), "19990101235960.003366+1200-");
  }

  // TODO: check of time zone and range
  /*   DateTime dt("20000101-1200-20001231-1200"); */

  {
    DateTime dt("2000-3000");
    EXPECT_TRUE(dt.is_range());
    EXPECT_TRUE(dt.lower());
    EXPECT_TRUE(dt.upper());

    EXPECT_EQ(dt.str(), "2000-3000");
  }
}

TEST(DateTimeTest, element_from_string_test) {
  DateTime dt = vega::from_string<DateTime>("20001231235959");
  EXPECT_EQ(dt.str(), "20001231235959");
}

TEST(DateTimeTest, to_string_test) {
  DateTime dt("2000-2020");
  EXPECT_EQ(vega::to_string(dt), std::string("2000-2020"));
}

TEST(DateTimeTest, manipulator_test) {
  vega::manipulators::PaddedStringManipulator<DateTime> manipulator{};
  manipulator.emplace_back(std::string("19001011"));
  manipulator.emplace_back(std::string("21000405123030"));

  EXPECT_EQ(manipulator[0].str(), "19001011");
  EXPECT_EQ(manipulator[1].str(), "21000405123030");
  EXPECT_EQ(manipulator.str(), std::string("19001011\\21000405123030 "));
}

TEST(DateTimeTest, stream_test) {
  std::vector<std::string> date_time_strings = {
    "2000",
    "-21000101001122",
    "9000-9999",
    "19991231235959.999999-",
    "20000101000000.000000-20991231235959.999999"
  };

  for (const auto& date_time_string : date_time_strings) {
    DateTime date_time(date_time_string);
    std::stringstream ss;

    ss << date_time;
    const std::string s = ss.str();
    EXPECT_EQ(s, date_time_string);

    ss = std::stringstream(s);
    DateTime date_time2;
    ss >> date_time2;

    EXPECT_EQ(date_time, date_time2);
  }

  std::string comma_separated_times = "19990415-20001225,2018-1400-2019+1400";
  std::stringstream ss(comma_separated_times);

  DateTime date_time;
  ss >> date_time;

  EXPECT_EQ(date_time.str(), "19990415-20001225");

  char c;
  ss >> c;
  EXPECT_EQ(c, ',');

  ss >> date_time;
  EXPECT_EQ(date_time.str(), "2018-1400-2019+1400");
}
