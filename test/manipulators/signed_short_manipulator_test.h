#pragma once

#include "../test_helper.h"

#include <sstream>
#include "vega/manipulators/signed_short_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(SignedShortManipulatorTest, constructor_test) {
  SignedShortManipulator manipulator{};

  manipulator.push_back(10);
  manipulator.push_back(-1000);

  EXPECT_EQ(manipulator.str(), "10\\-1000");

  SignedShortManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(SignedShortManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0046, 0x0135}, vr::SS};
  auto manipulator = data_element.get_manipulator<SignedShortManipulator>();

  manipulator->push_back(33);
  manipulator->push_back(-30303);

  EXPECT_EQ(data_element.str(), "33\\-30303");
}
