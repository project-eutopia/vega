#pragma once

#include "../test_helper.h"

#include "vega/manipulators/unlimited_text_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(UnlimitedTextManipulatorTest, constructor_test) {
  UnlimitedTextManipulator manipulator{};

  manipulator.value() = "hello world";

  EXPECT_EQ(manipulator.str(), "hello world ");

  UnlimitedTextManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator.value(), manipulator2.value());
}

TEST(UnlimitedTextManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0018, 0x1009}, vr::UT};
  auto manipulator = data_element.get_manipulator<UnlimitedTextManipulator>();

  manipulator->value() = "foo bar";

  EXPECT_EQ(data_element.str(), "foo bar ");
}
