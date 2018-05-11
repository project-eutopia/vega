#pragma once

#include "../test_helper.h"

#include "vega/dicom/file.h"
#include "vega/dicom/data_element.h"
#include "vega/manipulator.h"
#include <sstream>

using namespace vega;

TEST(DicomFileTest, dicom_file_test) {
  std::vector<std::string> file_names;

  file_names.push_back(tests::path_to_file("data/CT-MONO2-16-brain.dcm"));
  file_names.push_back(tests::path_to_file("data/0522c0002/CT.1.3.6.1.4.1.22213.2.26556.2.1.dcm"));
  file_names.push_back(tests::path_to_file("data/0522c0002/RP.1.2.246.352.71.5.4061753534.1331767.20170517120254.dcm"));
  file_names.push_back(tests::path_to_file("data/0522c0002/RS.1.2.246.352.71.4.4061753534.432046.20170517120331.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/CT_small.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/color-pl.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/color-px.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/ExplVR_BigEnd.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/image_dfl.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/MR_small.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/reportsi.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/rtdose.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/rtplan.dcm"));
  file_names.push_back(tests::path_to_file("data/undefined_SQ_rtdose.dcm"));

  for (const auto& file_name : file_names) {
    /* std::cout << file_name << std::endl; */
    dicom::File file(file_name);
    std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
    file.write(ss);
    dicom::File file_copy(ss);
    EXPECT_TRUE(*file.data_set() == *file_copy.data_set());
  }
}

TEST(DicomFileTest, dicom_file_nested_priv_sq_test) {
  std::string file_name = tests::path_to_file("data/pydicom/nested_priv_SQ.dcm");
  dicom::File file(file_name);
  std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
  file.write(ss);
  dicom::File file_copy(ss);
  EXPECT_TRUE(*file.data_set() == *file_copy.data_set());
}

// TODO
/* TEST(DicomFileTest, dicom_file_truncated_test) { */
/*   std::string file_name = tests::path_to_file("data/pydicom/rtplan_truncated.dcm"); */
/*   dicom::File file(file_name); */
/*   std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>(); */
/*   file.write(ss); */
/*   dicom::File file_copy(ss); */
/*   EXPECT_TRUE(*file.data_set() == *file_copy.data_set()); */
/* } */

TEST(DicomFileTest, dicom_file_allow_explicit_vrs_test) {
  std::vector<std::string> file_names;

  file_names.push_back(tests::path_to_file("data/pydicom/JPEG2000.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/JPEG-LL.dcm"));
  file_names.push_back(tests::path_to_file("data/pydicom/JPEG-lossy.dcm"));

  for (const auto& file_name : file_names) {
    dicom::File file(file_name, true);
    std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
    file.write(ss);
    dicom::File file_copy(ss, true);
    EXPECT_TRUE(*file.data_set() == *file_copy.data_set());
  }
}

TEST(DicomFileTest, private_sequence_test) {
  std::string file_name = tests::path_to_file("data/pydicom/priv_SQ.dcm");

  dicom::File file(file_name);
  EXPECT_GT(file.data_set()->size(), 0);

  std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
  file.write(ss);
  dicom::File file_copy(ss);
  EXPECT_TRUE(*file.data_set() == *file_copy.data_set());
}

TEST(DicomFileTest, no_group_length_meta_test) {
  std::string file_name = tests::path_to_file("data/pydicom/no_meta_group_length.dcm");

  dicom::File file(file_name);
  std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
  file.write(ss);
  dicom::File file_copy(ss);
  EXPECT_TRUE(*file.data_set() == *file_copy.data_set());

  auto data_element = file.data_set()->data_element(Tag{0x0008,0x0008});
  EXPECT_TRUE(bool(data_element)) << " no data_element in " << file_name;

  auto manipulator = data_element->get_manipulator<CS_Manipulator>();
  EXPECT_EQ((*manipulator)[0], "ORIGINAL") << " invalid content in " << file_name;
  EXPECT_EQ((*manipulator)[1], "PRIMARY") << " invalid content in " << file_name;
  EXPECT_EQ((*manipulator)[2], "PORTAL") << " invalid content in " << file_name;
}

TEST(DicomFileTest, no_meta_test) {
  // Capture error messages
  testing::internal::CaptureStderr();

  {
    std::vector<std::string> file_names;
    file_names.push_back(tests::path_to_file("data/pydicom/rtstruct.dcm"));
    file_names.push_back(tests::path_to_file("data/pydicom/ExplVR_BigEndNoMeta.dcm"));
    file_names.push_back(tests::path_to_file("data/pydicom/ExplVR_LitEndNoMeta.dcm"));

    for (const auto& file_name : file_names) {
      dicom::File file(file_name);
      std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
      file.write(ss);
      dicom::File file_copy(ss);
      EXPECT_TRUE(*file.data_set() == *file_copy.data_set());

      auto data_element = file.data_set()->data_element(Tag{0x0008,0x0005});
      EXPECT_TRUE(bool(data_element)) << " no data_element in " << file_name;
      if (!data_element) continue;

      auto manipulator = data_element->get_manipulator<CS_Manipulator>();
      EXPECT_EQ((*manipulator)[0], "ISO_IR 100") << " invalid content in " << file_name;
    }
  }

  std::string output = testing::internal::GetCapturedStderr();

  EXPECT_EQ(
    output,
    "Warning, failure reading file meta so assuming blank: In DataElement(Tag, VR), invalid VR of __ for tag (0008,0005), SpecificCharacterSet\n"
    "Warning, failure reading file meta so assuming blank: In DataElement(Tag, VR), invalid VR of __ for tag (0008,0005), SpecificCharacterSet\n"
    "Warning, failure reading file meta so assuming blank: In DataElement(Tag, VR), could not find dictionary page with tag = (0800,0500)\n"
    "Warning, trying to recover from error with transfer syntax Implicit VR Little Endian: In DataElement(Tag), could not find dictionary page with tag = (0800,0500)\n"
    "Warning, trying to recover from error with transfer syntax Explicit VR Little Endian: In DataElement(Tag, VR), could not find dictionary page with tag = (0800,0500)\n"
    "Warning, failure reading file meta so assuming blank: In DataElement(Tag, VR), could not find dictionary page with tag = (0800,0500)\n"
    "Warning, trying to recover from error with transfer syntax Implicit VR Little Endian: In DataElement(Tag), could not find dictionary page with tag = (0800,0500)\n"
    "Warning, trying to recover from error with transfer syntax Explicit VR Little Endian: In DataElement(Tag, VR), could not find dictionary page with tag = (0800,0500)\n"
    "Warning, failure reading file meta so assuming blank: Need TransferSyntaxUID element\n"
    "Warning, trying to recover from error with transfer syntax Implicit VR Little Endian: Reader encounted length too long to fit in file: tag=(0008,0005) length=676675 cur_pos=8 eof=434\n"
    "Warning, failure reading file meta so assuming blank: Need TransferSyntaxUID element\n"
    "Warning, trying to recover from error with transfer syntax Implicit VR Little Endian: Reader encounted length too long to fit in file: tag=(0008,0005) length=676675 cur_pos=8 eof=434\n"
  );
}

TEST(DicomFileTest, building_file_test) {
  dicom::File file(dicom::SOPClass("RT Plan Storage"));
  EXPECT_EQ(file.file_meta()->sop_class().name(), "RT Plan Storage");

  {
    auto data_element = std::make_shared<dicom::DataElement>("PatientName");
    auto manipulator = data_element->get_manipulator<manipulators::PersonNameManipulator>();
    manipulator->push_back("Smith^Alice");
    file.data_set()->add_data_element(data_element);
  }

  {
    auto referenced_frame_number = file.data_set()->new_element<dictionary::ReferencedFrameNumber>();
    referenced_frame_number->manipulator()->push_back(-456);
    referenced_frame_number->manipulator()->push_back(123456789);
  }

  std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
  file.write(ss);
  dicom::File file_copy(ss);
  EXPECT_TRUE(*file.data_set() == *file_copy.data_set());

  auto patient_name = file_copy.data_set()->data_element("PatientName");
  auto manipulator = patient_name->get_manipulator<PN_Manipulator>();
  EXPECT_EQ((*manipulator)[0], "Smith^Alice");

  auto referenced_frame_number = file_copy.data_set()->element<dictionary::ReferencedFrameNumber>();
  EXPECT_EQ((*referenced_frame_number->manipulator())[0], -456);
  EXPECT_EQ((*referenced_frame_number->manipulator())[1], 123456789);
}
