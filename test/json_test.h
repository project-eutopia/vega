#pragma once

#include "test_helper.h"
#include "vega/string.h"
#include "vega/tag.h"
#include "vega/json.h"
#include "vega/dicom/file.h"
#include "vega/dicom/data_set.h"
#include "vega/dicom/element.h"

using namespace vega;

TEST(ToJsonTest, string_test) {
  EXPECT_EQ(vega::to_json(std::string("hello world")), std::string("\"hello world\""));
}

TEST(ToJsonTest, floats_test) {
  EXPECT_EQ(vega::to_json((float)-1.234), std::string("-1.234"));
  EXPECT_EQ(vega::to_json((double)3e20), std::string("3e+20"));
}

TEST(ToJsonTest, ints_test) {
  EXPECT_EQ(vega::to_json((uint8_t)255), std::string("255"));
  EXPECT_EQ(vega::to_json((uint16_t)65535), std::string("65535"));
  EXPECT_EQ(vega::to_json((uint32_t)4000000000), std::string("4000000000"));
  EXPECT_EQ(vega::to_json((uint64_t)1234567890123456), std::string("1234567890123456"));
  EXPECT_EQ(vega::to_json((int8_t)-100), std::string("-100"));
  EXPECT_EQ(vega::to_json((int16_t)-30000), std::string("-30000"));
  EXPECT_EQ(vega::to_json((int32_t)-1000000000), std::string("-1000000000"));
  EXPECT_EQ(vega::to_json((int64_t)-1234567890123456), std::string("-1234567890123456"));
}

TEST(ToJsonTest, customs_test) {
  EXPECT_EQ(vega::to_json(Byte{.i = (int8_t)128}), std::string("128"));
  EXPECT_EQ(vega::to_json(Word{.i = 10000}), std::string("10000"));
  EXPECT_EQ(vega::to_json(Long{.i = 1000000}), std::string("1000000"));
  EXPECT_EQ(vega::to_json(DecimalString{123.456}), std::string("123.456"));
}

TEST(ToJsonTest, data_set_element_test) {
  auto data_set = std::make_shared<dicom::DataSet>();

  {
    auto element = data_set->new_element<vega::dictionary::PatientName>();
    element->manipulator()->push_back("Smith^Alice");
    element->manipulator()->push_back("Smith^Bob");
  }

  {
    auto element = data_set->new_element<vega::dictionary::TimeRange>();
    element->manipulator()->push_back(1.23);
    element->manipulator()->push_back(-3.14);
  }

  {
    auto element = data_set->new_element<vega::dictionary::PatientComments>();
    element->manipulator()->value() = "Very healthy";
  }

  {
    auto element = std::make_shared<dicom::DataElement>("FractionGroupSequence");

    {
      auto fraction_group = std::make_shared<dicom::DataSet>();
      auto fraction_group_number = std::make_shared<dicom::DataElement>("FractionGroupNumber");
      auto manipulator = fraction_group_number->get_manipulator<manipulators::IntegerStringManipulator>();
      manipulator->push_back(123);
      manipulator->push_back(456);
      fraction_group->add_data_element(fraction_group_number);
      element->data_sets().push_back(fraction_group);
    }

    {
      auto fraction_group = std::make_shared<dicom::DataSet>();
      auto fraction_group_number = std::make_shared<dicom::DataElement>("FractionGroupNumber");
      auto manipulator = fraction_group_number->get_manipulator<manipulators::IntegerStringManipulator>();
      manipulator->push_back(777);
      fraction_group->add_data_element(fraction_group_number);
      element->data_sets().push_back(fraction_group);
    }

    data_set->add_data_element(element);
  }

  std::stringstream ss;
  Formatter formatter(ss);
  data_set->json(formatter);

  EXPECT_EQ(ss.str(), std::string("{\"(0008,1163)\":[1.23,-3.14],\"(0010,0010)\":[\"Smith^Alice\",\"Smith^Bob\"],\"(0010,4000)\":\"Very healthy\",\"(300A,0070)\":[{\"(300A,0071)\":[123,456]},{\"(300A,0071)\":777}]}"));
}

TEST(FromJsonTest, value_from_json_test) {
  {
    std::stringstream ss("(00FF,1234)");
    Tag tag = Json::value_from_json<Tag>(ss);
    EXPECT_EQ(tag.group(), 0x00FF);
    EXPECT_EQ(tag.element(), 0x1234);
  }

  {
    std::stringstream ss("SQ");
    VR sq_vr = Json::value_from_json<VR>(ss);
    EXPECT_EQ(sq_vr, vr::SQ);
  }

  {
    std::stringstream ss("-555");
    int i = Json::value_from_json<int>(ss);
    EXPECT_EQ(i, -555);
  }

  {
    std::stringstream ss("123.456e-123");
    double d = Json::value_from_json<double>(ss);
    EXPECT_EQ(d, 123.456e-123);
  }

  {
    std::stringstream ss("\"Hello world!\\nHow are you??\"");
    std::string json = Json::value_from_json<std::string>(ss);
    EXPECT_EQ(json, "Hello world!\nHow are you??");
  }
}

TEST(FromJsonTest, data_set_element_test) {
  std::string json = "{\"(0008,1163)\":[1.23,-3.14],\"(0010,0010)\":[\"Smith^Alice\",\"Smith^Bob\"],\"(0010,4000)\":\"Very healthy\",\"(300A,0070)\":[{\"(300A,0071)\":[123,456]},{\"(300A,0071)\":777}]}";
  auto data_set = dicom::DataSet::from_json(json);

  EXPECT_EQ(data_set->size(), 4);

  {
    auto element = data_set->element<vega::dictionary::PatientName>();
    EXPECT_EQ(element->manipulator()->at(0), "Smith^Alice");
    EXPECT_EQ(element->manipulator()->at(1), "Smith^Bob");
  }

  {
    auto element = data_set->element<vega::dictionary::TimeRange>();
    EXPECT_EQ(element->manipulator()->at(0), 1.23);
    EXPECT_EQ(element->manipulator()->at(1), -3.14);
  }

  {
    auto element = data_set->element<vega::dictionary::PatientComments>();
    EXPECT_EQ(element->manipulator()->value(), "Very healthy");
  }
}

TEST(FullDicomJsonTest, test_equivalence) {
  std::vector<std::string> file_names;

  file_names.push_back(tests::path_to_file("data/pydicom/CT_small.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/rtplan.dcm"));

  for (const auto& file_name : file_names) {
    dicom::File file(file_name);

    std::stringstream original_json;
    Formatter formatter(original_json);
    file.data_set()->json(formatter);

    auto data_set_copy = dicom::DataSet::from_json(original_json);

    std::stringstream copied_json;
    Formatter formatter2(copied_json);
    data_set_copy->json(formatter2);

    EXPECT_EQ(original_json.str(), copied_json.str());
  }
}
