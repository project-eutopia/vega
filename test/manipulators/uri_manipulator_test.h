#pragma once

#include "../test_helper.h"

#include "vega/manipulators/uri_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(URIManipulatorTest, constructor_test) {
  URIManipulator manipulator{};

  manipulator.value() = "hello world";

  EXPECT_EQ(manipulator.str(), "hello world ");

  URIManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator.value(), manipulator2.value());
}

TEST(URIManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x0120}, vr::UR};
  auto manipulator = data_element.get_manipulator<URIManipulator>();

  manipulator->value() = "foo barr";

  EXPECT_EQ(data_element.str(), "foo barr");
}
