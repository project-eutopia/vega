#pragma once

#include "../test_helper.h"

#include <sstream>
#include "vega/manipulators/signed_long_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(SignedLongManipulatorTest, constructor_test) {
  SignedLongManipulator manipulator{};

  manipulator.push_back(10);
  manipulator.push_back(-1000);

  EXPECT_EQ(manipulator.str(), "10\\-1000");

  SignedLongManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(SignedLongManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0040, 0xA162}, vr::SL};
  auto manipulator = data_element.get_manipulator<SignedLongManipulator>();

  manipulator->push_back(33);
  manipulator->push_back(-65536);

  EXPECT_EQ(data_element.str(), "33\\-65536");
}
