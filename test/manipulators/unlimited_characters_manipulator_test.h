#pragma once

#include "../test_helper.h"

#include "vega/manipulators/unlimited_characters_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(UnlimitedCharactersManipulatorTest, constructor_test) {
  UnlimitedCharactersManipulator manipulator{};

  manipulator.push_back("hello world");
  manipulator.push_back("foo bar");

  EXPECT_EQ(manipulator.str(), "hello world\\foo bar ");

  UnlimitedCharactersManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(UnlimitedCharactersManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x0119}, vr::UC};
  auto manipulator = data_element.get_manipulator<UnlimitedCharactersManipulator>();

  manipulator->push_back("qwerty");
  manipulator->push_back("howdy");

  EXPECT_EQ(data_element.str(), "qwerty\\howdy");
}
