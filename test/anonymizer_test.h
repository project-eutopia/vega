#pragma once

#include "test_helper.h"

#include "vega/anonymizer.h"

using namespace vega;

TEST(AnonymizerTest, test_anonymizer) {
  Anonymizer a{"test_id7531", [](dicom::DataElement& e) -> bool {
    // Remove Rows
    if (e.tag() == dictionary::Rows::tag) return true;
    // Change Columns to 777
    if (e.tag() == dictionary::Columns::tag) {
      e.get_manipulator<typename dictionary::Columns::manipulator_type>()->at(0) = 777;
    }
    return false;
  }};

  std::string filename = tests::path_to_file("data/undefined_SQ_rtdose.dcm");
  dicom::File file(filename);

  // Remove anonymous data
  file.data_set()->erase<dictionary::PatientIdentityRemoved>();
  file.data_set()->erase<dictionary::DeidentificationMethod>();

  // Add private group
  auto private_data_creator = std::make_shared<dicom::DataElement>(Tag{0x0009,0x0044}, file.data_set());
  private_data_creator->get_manipulator<LO_Manipulator>()->push_back("GEMS_GENIE_1");
  file.data_set()->add_data_element(private_data_creator);

  auto study_name = std::make_shared<dicom::DataElement>(Tag{0x0009,0x04410}, file.data_set());
  study_name->get_manipulator<LO_Manipulator>()->push_back("Private study name");
  file.data_set()->add_data_element(study_name);
  EXPECT_TRUE(file.data_set()->data_element(Tag{0x0009,0x0044}));

  std::string original_patient_name = file.data_set()->element<dictionary::PatientName>()->manipulator()->at(0);

  EXPECT_TRUE(file.data_set()->element<dictionary::Rows>());
  EXPECT_EQ(file.data_set()->element<dictionary::Columns>()->manipulator()->at(0), 213);

  a.anonymize(file);

  EXPECT_NE(
    original_patient_name,
    file.data_set()->element<dictionary::PatientName>()->manipulator()->at(0)
  );

  EXPECT_EQ(
    "test_id7531",
    file.data_set()->element<dictionary::PatientID>()->manipulator()->at(0)
  );
  EXPECT_EQ(
    "test_id7531",
    file.data_set()->element<dictionary::PatientName>()->manipulator()->at(0)
  );

  EXPECT_EQ(file.data_set()->element<dictionary::PatientBirthDate>()->manipulator()->size(), 0);
  EXPECT_EQ(file.data_set()->element<dictionary::PatientSex>()->manipulator()->size(), 0);

  EXPECT_FALSE(file.data_set()->element<dictionary::Rows>());
  EXPECT_EQ(file.data_set()->element<dictionary::Columns>()->manipulator()->at(0), 777);

  EXPECT_EQ(file.data_set()->element<dictionary::PatientIdentityRemoved>()->manipulator()->at(0), "YES");
  EXPECT_EQ(file.data_set()->element<dictionary::DeidentificationMethod>()->manipulator()->at(0), "vega C++ Library Anonymizer class");
  EXPECT_EQ(file.data_set()->element<dictionary::DeidentificationMethod>()->manipulator()->at(1), "vega C++ Library custom anonymization function");

  EXPECT_FALSE(file.data_set()->data_element(Tag{0x0009,0x0044}));
}

TEST(AnonymizerTest, test_anonymizer_patient_id_same_across_multiple_calls) {
  Anonymizer a;

  std::string filename = tests::path_to_file("data/undefined_SQ_rtdose.dcm");
  dicom::File file(filename);
  a.anonymize(file);

  auto patient_id = file.data_set()->element<dictionary::PatientID>()->manipulator()->at(0);

  a.anonymize(file);
  EXPECT_EQ(patient_id, file.data_set()->element<dictionary::PatientName>()->manipulator()->at(0));
  EXPECT_EQ(patient_id, file.data_set()->element<dictionary::PatientID>()->manipulator()->at(0));
}
