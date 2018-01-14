#pragma once

#include "../test_helper.h"

#include "vega/manipulators/person_name_manipulator.h"
#include "vega/dicom/data_element.h"

using namespace vega;
using namespace vega::manipulators;

TEST(PersonNameManipulatorTest, constructor_test) {
  PersonNameManipulator manipulator{};

  manipulator.push_back("Alice Smith");
  manipulator.push_back("Bob Smith");

  EXPECT_EQ(manipulator.str(), "Alice Smith\\Bob Smith ");

  PersonNameManipulator manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}

TEST(PersonNameManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0010, 0x0010}, vr::PN};
  auto manipulator = data_element.get_manipulator<PersonNameManipulator>();

  manipulator->push_back("foo");
  manipulator->push_back("bar");

  EXPECT_EQ(data_element.str(), "foo\\bar ");
}
