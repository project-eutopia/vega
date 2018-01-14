#pragma once

#include "../test_helper.h"

#include <sstream>
#include "vega/manipulators/code_string_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(CodeStringManipulatorTest, constructor_test) {
  CodeStringManipulator manipulator{};

  manipulator.push_back("hello");
  manipulator.push_back("world");

  EXPECT_EQ(manipulator.str(), "hello\\world ");

  CodeStringManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(CodeStringManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x0008}, vr::CS};
  auto manipulator = data_element.get_manipulator<CodeStringManipulator>();

  manipulator->push_back("foo");
  manipulator->push_back("bar");

  EXPECT_EQ(data_element.str(), "foo\\bar ");
}
