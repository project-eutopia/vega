#pragma once

#include "../test_helper.h"

#include "vega/dicom/data_set.h"
#include "vega/manipulators/person_name_manipulator.h"
#include "vega/manipulators/integer_string_manipulator.h"

using namespace vega;

TEST(DataSetTest, data_set_test) {
  auto data_set = std::make_shared<dicom::DataSet>();

  {
    auto element = std::make_shared<dicom::DataElement>("PatientName");
    auto manipulator = element->get_manipulator<manipulators::PersonNameManipulator>();
    manipulator->push_back("Smith^Alice");
    data_set->add_data_element(element);
  }

  {
    auto element = std::make_shared<dicom::DataElement>("FractionGroupSequence");

    auto fraction_group = std::make_shared<dicom::DataSet>();
    auto fraction_group_number = std::make_shared<dicom::DataElement>("FractionGroupNumber");
    auto manipulator = fraction_group_number->get_manipulator<manipulators::IntegerStringManipulator>();
    manipulator->push_back(123);
    fraction_group->add_data_element(fraction_group_number);

    element->data_sets().push_back(fraction_group);
    data_set->add_data_element(element);
  }
}

TEST(DataSetTest, data_set_element_test) {
  auto data_set = std::make_shared<dicom::DataSet>();

  {
    auto element = data_set->new_element<vega::dictionary::PatientName>();
    element->manipulator()->push_back("Smith^Alice");
  }

  {
    auto element = data_set->new_element<vega::dictionary::TimeRange>();
    element->manipulator()->push_back(1.23);
    element->manipulator()->push_back(-3.14);
  }

  {
    auto element = data_set->element<vega::dictionary::Rows>();
    EXPECT_FALSE(bool(element));
  }

  EXPECT_EQ(data_set->element<vega::dictionary::PatientName>()->manipulator()->front(), std::string("Smith^Alice"));
  EXPECT_EQ(data_set->element<vega::dictionary::TimeRange>()->manipulator()->back(), -3.14);
}

TEST(DataSetTest, data_set_remove_test) {
  auto data_set = std::make_shared<dicom::DataSet>();
  data_set->new_element<vega::dictionary::PatientName>();
  data_set->new_element<vega::dictionary::Rows>();
  data_set->new_element<vega::dictionary::Columns>();
  auto planes = data_set->new_element<vega::dictionary::Planes>();

  EXPECT_NE(data_set->element<vega::dictionary::PatientName>(), nullptr);
  data_set->erase(data_set->data_element(Tag{0x0010,0x0010}));
  EXPECT_EQ(data_set->element<vega::dictionary::PatientName>(), nullptr);

  EXPECT_NE(data_set->data_element(vega::dictionary::Rows::tag), nullptr);
  data_set->erase(vega::dictionary::Rows::tag);
  EXPECT_EQ(data_set->data_element(vega::dictionary::Rows::tag), nullptr);

  EXPECT_NE(data_set->data_element(vega::dictionary::Columns::tag), nullptr);
  data_set->erase(vega::dictionary::Columns::tag_mask);
  EXPECT_EQ(data_set->data_element(vega::dictionary::Columns::tag), nullptr);

  EXPECT_NE(data_set->data_element(vega::dictionary::Planes::tag), nullptr);
  data_set->erase(planes);
  EXPECT_EQ(data_set->data_element(vega::dictionary::Planes::tag), nullptr);

  data_set->new_element<vega::dictionary::InspectionSelectionCriteria>();
  data_set->new_element<vega::dictionary::CurveDimensions>(Tag{0x5012,0x0005});

  EXPECT_NE(data_set->element<vega::dictionary::InspectionSelectionCriteria>(), nullptr);
  data_set->erase<vega::dictionary::InspectionSelectionCriteria>();
  EXPECT_EQ(data_set->element<vega::dictionary::InspectionSelectionCriteria>(), nullptr);

  EXPECT_NE(data_set->element<vega::dictionary::CurveDimensions>(), nullptr);
  data_set->erase<vega::dictionary::CurveDimensions>();
  EXPECT_EQ(data_set->element<vega::dictionary::CurveDimensions>(), nullptr);
}
