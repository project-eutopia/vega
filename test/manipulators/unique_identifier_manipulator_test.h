#pragma once

#include "../test_helper.h"

#include "vega/manipulators/unique_identifier_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(UniqueIdentifierManipulatorTest, constructor_test) {
  UniqueIdentifierManipulator manipulator{};

  manipulator.uid() = UID("hello world");
  EXPECT_EQ(manipulator.uid().str(), std::string("hello world\0"));

  UniqueIdentifierManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator.uid(), manipulator2.uid());
}

TEST(UniqueIdentifierManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0008, 0x0016}, vr::UI};
  auto manipulator = data_element.get_manipulator<UniqueIdentifierManipulator>();

  manipulator->uid() = UID("foo bar");

  EXPECT_EQ(data_element.str(), std::string("foo bar\0"));
}
