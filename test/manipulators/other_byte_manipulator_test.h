#pragma once

#include "../test_helper.h"

#include "vega/manipulators/other_byte_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(OtherByteManipulatorTest, constructor_test) {
  OtherByteManipulator manipulator{};

  manipulator.push_back(Byte{.u=0});
  manipulator.push_back(Byte{.u=255});

  EXPECT_EQ(manipulator.str(), "0x00\\0xff");

  OtherByteManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(OtherByteManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0014, 0x3080}, vr::OB};
  auto manipulator = data_element.get_manipulator<OtherByteManipulator>();

  manipulator->push_back(Byte{.u=10});
  manipulator->push_back(Byte{.u=100});

  EXPECT_EQ(data_element.str(), "0x0a\\0x64");
}
