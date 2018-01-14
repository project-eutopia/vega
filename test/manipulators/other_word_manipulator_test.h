#pragma once

#include "../test_helper.h"

#include "vega/manipulators/other_word_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(OtherWordManipulatorTest, constructor_test) {
  OtherWordManipulator manipulator{};

  manipulator.push_back(Word{.u=256});
  manipulator.push_back(Word{.u=65535});

  EXPECT_EQ(manipulator.str(), "0x0100\\0xffff");

  OtherWordManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(OtherWordManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0028, 0x1201}, vr::OW};
  auto manipulator = data_element.get_manipulator<OtherWordManipulator>();

  manipulator->push_back(Word{.u=0xfa01});
  manipulator->push_back(Word{.u=0x0123});

  EXPECT_EQ(data_element.str(), "0xfa01\\0x0123");
}
