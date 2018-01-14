#pragma once

#include "../test_helper.h"

#include <sstream>
#include "vega/manipulators/date_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(DateManipulatorTest, constructor_test) {
  DateManipulator manipulator{};

  manipulator.emplace_back(Date{"20171124"});
  manipulator.emplace_back(Date{"20000101"});

  EXPECT_EQ(manipulator.str(), "20171124\\20000101 ");

  DateManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(DateManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x0020}, vr::DA};
  auto manipulator = data_element.get_manipulator<DateManipulator>();

  manipulator->push_back(Date{"19000615"});
  manipulator->push_back(Date{"22001231"});

  EXPECT_EQ(data_element.str(), "19000615\\22001231 ");
}
