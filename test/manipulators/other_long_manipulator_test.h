#pragma once

#include "../test_helper.h"

#include "vega/manipulators/other_long_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(OtherLongManipulatorTest, constructor_test) {
  OtherLongManipulator manipulator{};

  manipulator.push_back(Long{.u = 65536});
  manipulator.push_back(Long{.i = -65536});

  EXPECT_EQ(manipulator.str(), "0x00010000\\0xffff0000");

  OtherLongManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(OtherLongManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0066, 0x0040}, vr::OL};
  auto manipulator = data_element.get_manipulator<OtherLongManipulator>();

  manipulator->push_back(Long{.u = 1});
  manipulator->push_back(Long{.i = -2});

  EXPECT_EQ(data_element.str(), "0x00000001\\0xfffffffe");
}
