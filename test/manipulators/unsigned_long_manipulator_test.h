#pragma once

#include "../test_helper.h"

#include <sstream>
#include "vega/manipulators/unsigned_long_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(UnsignedLongManipulatorTest, constructor_test) {
  UnsignedLongManipulator manipulator{};

  manipulator.push_back(10);
  manipulator.push_back(1000);

  EXPECT_EQ(manipulator.str(), "10\\1000");

  UnsignedLongManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(UnsignedLongManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0002, 0x0000}, vr::UL};
  auto manipulator = data_element.get_manipulator<UnsignedLongManipulator>();

  manipulator->push_back(33);
  manipulator->push_back(65536);

  EXPECT_EQ(data_element.str(), "33\\65536");
}
