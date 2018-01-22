#pragma once

#include "../test_helper.h"

#include "vega/manipulators/padded_string_manipulator.h"

using namespace vega;
using namespace vega::manipulators;

TEST(PaddedStringManipulatorTest, string_test) {
  PaddedStringManipulator<std::string> manipulator{};

  manipulator.push_back(std::string("hello"));
  manipulator.push_back(std::string("world"));

  EXPECT_EQ(manipulator.str(), std::string("hello\\world "));
}

TEST(PaddedStringManipulatorTest, int_test) {
  PaddedStringManipulator<int32_t> manipulator{};

  manipulator.push_back(-45);
  manipulator.push_back(100);

  EXPECT_EQ(manipulator.str(), std::string("-45\\100 "));
}
