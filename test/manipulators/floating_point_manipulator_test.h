#pragma once

#include "../test_helper.h"

#include "vega/manipulators/floating_point_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(FloatingPointManipulatorTest, constructor_test) {
  FloatingPointManipulator manipulator{};

  manipulator.emplace_back(1.234);
  manipulator.emplace_back(-50000);

  EXPECT_EQ(manipulator.str(), "1.234000e+00\\-5.000000e+04");

  FloatingPointManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(FloatingPointManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0018, 0x605A}, vr::FL};
  auto manipulator = data_element.get_manipulator<FloatingPointManipulator>();

  manipulator->push_back(0.0000001);
  manipulator->push_back(-5.55e25);

  EXPECT_EQ(data_element.str(), "1.000000e-07\\-5.550000e+25");
}
