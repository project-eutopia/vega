#pragma once

#include "test_helper.h"

#include "vega/string.h"
#include "vega/date.h"
#include "vega/manipulators/padded_string_manipulator.h"

using namespace vega;

TEST(DateTest, constructor_test) {
  {
    Date date("15001231");
  }

  {
    Date date("20000105");
  }
}

TEST(DateTest, range_test) {
  {
    Date date(" - 22221111 ");
    EXPECT_TRUE(date.is_range());
    EXPECT_FALSE(date.lower());
    EXPECT_TRUE(date.upper());

    EXPECT_EQ(date.str(), "-22221111");
  }

  {
    Date date(" 19900408 - ");
    EXPECT_TRUE(date.is_range());
    EXPECT_TRUE(date.lower());
    EXPECT_FALSE(date.upper());

    EXPECT_EQ(date.str(), "19900408-");
  }

  {
    Date date(" 1999.01.04 - 20100909 ");
    EXPECT_TRUE(date.is_range());
    EXPECT_TRUE(date.lower());
    EXPECT_TRUE(date.upper());

    EXPECT_EQ(date.lower()->str(), "1999.01.04");
    EXPECT_EQ(date.str(), "1999.01.04-20100909");
  }
}

TEST(DateTest, old_acr_nema_standard_test) {
  {
    Date date("2017.11.22");
  }

  {
    Date date("1999.01.02");
  }
}

TEST(DateTest, element_from_string_test) {
  Date date = vega::from_string<Date>("19991231");
}

TEST(DateTest, to_string_test) {
  Date date = vega::from_string<Date>("20000303");
  EXPECT_EQ(vega::to_string(date), std::string("20000303"));
}

TEST(DateTest, manipulator_test) {
  vega::manipulators::PaddedStringManipulator<Date, ' ', '\\'> date_manipulator{};
  date_manipulator.emplace_back(std::string("19001011"));
  date_manipulator.emplace_back(std::string("21000405"));

  EXPECT_EQ(date_manipulator[0].str(), "19001011");
  EXPECT_EQ(date_manipulator[1].str(), "21000405");
  EXPECT_EQ(date_manipulator.str(), std::string("19001011\\21000405 "));
}
