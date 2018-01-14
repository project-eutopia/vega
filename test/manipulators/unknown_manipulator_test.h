#pragma once

#include "../test_helper.h"

#include "vega/manipulators/unknown_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(UnknownManipulatorTest, constructor_test) {
  UnknownManipulator manipulator{};

  manipulator.push_back(Byte{.u=0});
  manipulator.push_back(Byte{.u=255});

  EXPECT_EQ(manipulator.str(), "0x00\\0xff");

  UnknownManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(UnknownManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0072, 0x006d}, vr::UN};
  auto manipulator = data_element.get_manipulator<UnknownManipulator>();

  manipulator->push_back(Byte{.u=10});
  manipulator->push_back(Byte{.u=100});

  EXPECT_EQ(data_element.str(), "0x0a\\0x64");
}
