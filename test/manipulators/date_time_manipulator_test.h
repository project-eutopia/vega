#pragma once

#include "../test_helper.h"

#include <sstream>
#include "vega/manipulators/date_time_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(DateTimeManipulatorTest, constructor_test) {
  DateTimeManipulator manipulator{};

  manipulator.emplace_back(DateTime{"199010"});
  manipulator.emplace_back(DateTime{"20100415120000.123456+1200"});

  EXPECT_EQ(manipulator.str(), "199010\\20100415120000.123456+1200 ");

  DateTimeManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(DateTimeManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x0015}, vr::DT};
  auto manipulator = data_element.get_manipulator<DateTimeManipulator>();

  manipulator->push_back(DateTime{"99991231"});
  manipulator->push_back(DateTime{"2020010112"});

  EXPECT_EQ(data_element.str(), "99991231\\2020010112 ");
}
