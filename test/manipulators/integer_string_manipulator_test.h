#pragma once

#include "../test_helper.h"

#include "vega/manipulators/integer_string_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(IntegerStringManipulatorTest, constructor_test) {
  IntegerStringManipulator manipulator{};

  manipulator.emplace_back(-16);
  manipulator.emplace_back(123456789);

  EXPECT_EQ(manipulator.str(), "-16\\123456789 ");

  IntegerStringManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(IntegerStringManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x1160}, vr::IS};
  auto manipulator = data_element.get_manipulator<IntegerStringManipulator>();

  manipulator->push_back(0);
  manipulator->push_back(-9990001);

  EXPECT_EQ(data_element.str(), "0\\-9990001");
}
