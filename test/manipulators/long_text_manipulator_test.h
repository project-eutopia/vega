#pragma once

#include "../test_helper.h"

#include "vega/manipulators/long_text_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(LongTextManipulatorTest, constructor_test) {
  LongTextManipulator manipulator{};

  manipulator.value() = "hello world";

  EXPECT_EQ(manipulator.str(), "hello world ");

  LongTextManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator.value(), manipulator2.value());
}

TEST(LongTextManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x4000}, vr::LT};
  auto manipulator = data_element.get_manipulator<LongTextManipulator>();

  manipulator->value() = "the quick brown fox jumped over the lazy dog";

  EXPECT_EQ(data_element.str(), "the quick brown fox jumped over the lazy dog");
}
