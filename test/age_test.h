#pragma once

#include "test_helper.h"

#include "vega/string.h"
#include "vega/age.h"
#include "vega/manipulators/padded_string_manipulator.h"

using namespace vega;

TEST(AgeTest, constructor_test) {
  {
    Age age(123, Age::Unit::DAY);
    EXPECT_EQ(vega::to_string(age), std::string("123D"));
  }

  {
    Age age(10, Age::Unit::WEEK);
    EXPECT_EQ(vega::to_string(age), std::string("010W"));
  }

  {
    Age age(999, Age::Unit::MONTH);
    EXPECT_EQ(vega::to_string(age), std::string("999M"));
  }

  {
    Age age(0, Age::Unit::YEAR);
    EXPECT_EQ(vega::to_string(age), std::string("000Y"));
  }
}

TEST(AgeTest, equality_test) {
  EXPECT_EQ(Age(365, Age::Unit::DAY), Age(365, Age::Unit::DAY));
  EXPECT_NE(Age(1, Age::Unit::YEAR), Age(365, Age::Unit::DAY));
  EXPECT_NE(Age(365, Age::Unit::YEAR), Age(365, Age::Unit::DAY));
  EXPECT_NE(Age(1, Age::Unit::DAY), Age(2, Age::Unit::DAY));
}

TEST(AgeTest, element_from_string_test) {
  Age age = vega::from_string<Age>("500W");
  EXPECT_EQ(age.count(), 500);
  EXPECT_EQ(age.unit(), Age::Unit::WEEK);
}

TEST(AgeTest, manipulator_test) {
  vega::manipulators::PaddedStringManipulator<Age> age_manipulator{};
  age_manipulator.emplace_back(111, Age::Unit::DAY);
  age_manipulator.emplace_back(5, Age::Unit::YEAR);

  EXPECT_EQ(age_manipulator[0].unit(), Age::Unit::DAY);
  EXPECT_EQ(age_manipulator[1].count(), 5);
  EXPECT_EQ(age_manipulator.str(), std::string("111D\\005Y "));
}
