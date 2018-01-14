#pragma once

#include "../test_helper.h"

#include "vega/manipulators/other_float_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(OtherFloatManipulatorTest, constructor_test) {
  OtherFloatManipulator manipulator{};

  manipulator.push_back(-2.71);
  manipulator.push_back(1e20);

  EXPECT_EQ(manipulator.str(), "-2.710000e+00\\1.000000e+20");

  OtherFloatManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(OtherFloatManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x7FE0, 0x0008}, vr::OF};
  auto manipulator = data_element.get_manipulator<OtherFloatManipulator>();

  manipulator->push_back(-7.77e-21);
  manipulator->push_back(0.0);

  EXPECT_EQ(data_element.str(), "-7.770000e-21\\0.000000e+00");
}
