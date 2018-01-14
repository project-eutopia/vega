#pragma once

#include "../test_helper.h"

#include "vega/manipulators/short_string_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(ShortStringManipulatorTest, constructor_test) {
  ShortStringManipulator manipulator{};

  manipulator.push_back("Alice Smith");
  manipulator.push_back("Bob Smith");

  EXPECT_EQ(manipulator.str(), "Alice Smith\\Bob Smith ");

  ShortStringManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(ShortStringManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x0050}, vr::SH};
  auto manipulator = data_element.get_manipulator<ShortStringManipulator>();

  manipulator->push_back("foo");
  manipulator->push_back("bar");

  EXPECT_EQ(data_element.str(), "foo\\bar ");
}
