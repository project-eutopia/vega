#pragma once

#include "../test_helper.h"

#include "vega/manipulators/other_double_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(OtherDoubleManipulatorTest, constructor_test) {
  OtherDoubleManipulator manipulator{};

  manipulator.push_back(-2.71);
  manipulator.push_back(1e100);

  EXPECT_EQ(manipulator.str(), "-2.71e0\\1e100");

  OtherDoubleManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(OtherDoubleManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x7FE0, 0x0009}, vr::OD};
  auto manipulator = data_element.get_manipulator<OtherDoubleManipulator>();

  manipulator->push_back(-7.77e-77);
  manipulator->push_back(0.0);

  EXPECT_EQ(data_element.str(), "-7.77e-77\\0e0");
}
