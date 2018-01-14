#pragma once

#include "../test_helper.h"

#include "vega/manipulators/time_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(TimeManipulatorTest, constructor_test) {
  TimeManipulator manipulator{};

  manipulator.emplace_back(Time{"23"});
  manipulator.emplace_back(Time{"120000.123456"});

  EXPECT_EQ(manipulator.str(), "23\\120000.123456");

  TimeManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(TimeManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x0030}, vr::TM};
  auto manipulator = data_element.get_manipulator<TimeManipulator>();

  manipulator->emplace_back(Time{"000000.100000"});
  manipulator->emplace_back(Time{"235959.999999"});

  EXPECT_EQ(data_element.str(), "000000.100000\\235959.999999 ");
}
