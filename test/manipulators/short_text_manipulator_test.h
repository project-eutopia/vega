#pragma once

#include "../test_helper.h"

#include "vega/manipulators/short_text_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(ShortTextManipulatorTest, constructor_test) {
  ShortTextManipulator manipulator{};

  manipulator.value() = "hello world";

  EXPECT_EQ(manipulator.str(), "hello world ");

  ShortTextManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator.value(), manipulator2.value());
}

TEST(ShortTextManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x0081}, vr::ST};
  auto manipulator = data_element.get_manipulator<ShortTextManipulator>();

  manipulator->value() = "foo bar";

  EXPECT_EQ(data_element.str(), "foo bar ");
}
