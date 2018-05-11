#pragma once

#include "../test_helper.h"

#include "vega/dicom/data_element.h"

using namespace vega;

TEST(DataElementTest, constructor_with_name_test) {
  try {
    dicom::DataElement element{"DoesNotExist"};
    EXPECT_TRUE(false);
  } catch(const vega::Exception& ex) {
    EXPECT_EQ(std::string(ex.what()), std::string("Cannot find dictionary page with name DoesNotExist"));
  }

  try {
    dicom::DataElement element{"VariablePixelData"};
    EXPECT_TRUE(false);
  } catch(const vega::Exception& ex) {
    EXPECT_EQ(std::string(ex.what()), std::string("DataElement(string), must pass VR for ambiguous VR when set to explicit, name = VariablePixelData"));
  }

  dicom::DataElement element{"PatientName"};
  EXPECT_EQ(element.tag(), vega::Tag(0x0010,0x0010));
  EXPECT_EQ(element.vr(), vr::PN);
}

TEST(DataElementTest, constructor_test) {
  try {
    dicom::DataElement element{Tag{0x1234, 0xfafa}};
    EXPECT_TRUE(false);
  } catch(const vega::Exception& ex) {
    EXPECT_EQ(std::string(ex.what()), std::string("In DataElement(Tag), could not find dictionary page with tag = (1234,FAFA)"));
  }

  try {
    // Pixel data has VR = OB/OW
    dicom::DataElement element{Tag{0x7fe0, 0x0010}};
    EXPECT_TRUE(false);
  } catch(const vega::Exception& ex) {
    EXPECT_EQ(std::string(ex.what()), std::string("DataElement(Tag), must pass VR for ambiguous VR when set to explicit, tag = (7FE0,0010)"));
  }

  EXPECT_EQ(dicom::DataElement(Tag{0x7fe0,0x0010}, nullptr, true).vr(), vr::OW);
  EXPECT_EQ(dicom::DataElement(std::string("PixelData"), nullptr, true).vr(), vr::OW);
  EXPECT_EQ(dicom::DataElement(Tag{0x5000,0x200c}, nullptr, true).vr(), vr::OX);
  try {
    // Pixel data has VR = OB/OW
    dicom::DataElement element(std::string("AudioSampleData"), nullptr, true);
    EXPECT_TRUE(false);
  } catch(const vega::Exception& ex) {
    EXPECT_EQ(std::string(ex.what()), std::string("Cannot initialize DataElement with dictionary page name that has ambiguous tag: AudioSampleData"));
  }

  {
    dicom::DataElement patient_name{Tag{0x0010, 0x0010}, vr::PN};
    EXPECT_EQ(patient_name.tag(), Tag(0x0010, 0x0010));
    EXPECT_EQ(patient_name.vr(), vr::PN);
  }

  try {
    dicom::DataElement element{Tag{0x7fe0, 0x0010}, vr::OX};
    EXPECT_TRUE(false);
  } catch(const vega::Exception& ex) {
    EXPECT_EQ(std::string(ex.what()), std::string("DataElement cannot be constructed with multi-VR of ox"));
  }

  try {
    dicom::DataElement element{Tag{0x1234, 0xfafa}, vr::UN};
    EXPECT_TRUE(false);
  } catch(const vega::Exception& ex) {
    EXPECT_EQ(std::string(ex.what()), std::string("In DataElement(Tag, VR), could not find dictionary page with tag = (1234,FAFA)"));
  }

  try {
    // Patient name has VR of PN
    dicom::DataElement element{Tag{0x0010, 0x0010}, vr::LO};
    EXPECT_TRUE(false);
  } catch(const vega::Exception& ex) {
    EXPECT_EQ(std::string(ex.what()), std::string("In DataElement(Tag, VR), invalid VR of LO for tag (0010,0010), PatientName"));
  }

  {
    dicom::DataElement patient_name{Tag{0x0010, 0x0010}};
    EXPECT_EQ(patient_name.tag(), Tag(0x0010, 0x0010));
    EXPECT_EQ(patient_name.vr(), vr::PN);
  }
}
