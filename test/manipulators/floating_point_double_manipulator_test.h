#pragma once

#include "../test_helper.h"

#include "vega/manipulators/floating_point_double_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(FloatingPointDoubleManipulatorTest, constructor_test) {
  FloatingPointDoubleManipulator manipulator{};

  manipulator.emplace_back(1.234);
  manipulator.emplace_back(-50000);

  EXPECT_EQ(manipulator.str(), "1.234e0\\-5e4");

  FloatingPointDoubleManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(FloatingPointDoubleManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0018, 0x1271}, vr::FD};
  auto manipulator = data_element.get_manipulator<FloatingPointDoubleManipulator>();

  manipulator->push_back(0.0000001);
  manipulator->push_back(-5.55e55);

  EXPECT_EQ(data_element.str(), "1e-7\\-5.55e55");
}
