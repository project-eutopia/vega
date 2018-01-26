#pragma once

#include "../test_helper.h"

#include <sstream>
#include "vega/manipulators/attribute_tag_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(AttributeTagManipulatorTest, constructor_test) {
  AttributeTagManipulator manipulator{};

  manipulator.emplace_back(0x1234, 0xffff);
  manipulator.emplace_back(0x8000, 0x0001);

  EXPECT_EQ(manipulator.str(), "(1234,FFFF)\\(8000,0001)");

  AttributeTagManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(AttributeTagManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0020, 0x5000}, vr::AT};
  auto manipulator = data_element.get_manipulator<AttributeTagManipulator>();

  manipulator->emplace_back(0x0000, 0x0000);
  manipulator->emplace_back(0x9900, 0xabcd);

  EXPECT_EQ(data_element.str(), "(0000,0000)\\(9900,ABCD)");
}
