#pragma once

#include "../test_helper.h"

#include "vega/manipulators/unsigned_short_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(UnsignedShortManipulatorTest, constructor_test) {
  UnsignedShortManipulator manipulator{};

  manipulator.push_back(10);
  manipulator.push_back(1000);

  EXPECT_EQ(manipulator.str(), "10\\1000");

  UnsignedShortManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(UnsignedShortManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0004, 0x1212}, vr::US};
  auto manipulator = data_element.get_manipulator<UnsignedShortManipulator>();

  manipulator->push_back(33);
  manipulator->push_back(65535);

  EXPECT_EQ(data_element.str(), "33\\65535");
}
