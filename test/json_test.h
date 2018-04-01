#pragma once

#include "test_helper.h"
#include "vega/string.h"
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
  }

  {
    auto element = data_set->new_element<vega::dictionary::TimeRange>();
    element->manipulator()->push_back(1.23);
    element->manipulator()->push_back(-3.14);
  }

  std::stringstream ss;
  Formatter formatter(ss);
  data_set->json(formatter);

  EXPECT_EQ(ss.str(), std::string("{\"(0008,1163)\":[1.23,-3.14],\"(0010,0010)\":\"Smith^Alice\"}"));
}
