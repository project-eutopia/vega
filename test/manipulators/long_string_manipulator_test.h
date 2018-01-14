#pragma once

#include "../test_helper.h"

#include "vega/manipulators/long_string_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(LongStringManipulatorTest, constructor_test) {
  LongStringManipulator manipulator{};

  manipulator.emplace_back("hello");
  manipulator.emplace_back("world");

  EXPECT_EQ(manipulator.str(), "hello\\world ");

  LongStringManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(LongStringManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x0070}, vr::LO};
  auto manipulator = data_element.get_manipulator<LongStringManipulator>();

  manipulator->push_back("the quick brown fox");
  manipulator->push_back("jumped over the lazy dog");

  EXPECT_EQ(data_element.str(), "the quick brown fox\\jumped over the lazy dog");
}
