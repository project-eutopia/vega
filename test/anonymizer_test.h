#pragma once

#include "test_helper.h"

#include "vega/anonymizer.h"

using namespace vega;

TEST(AnonymizerTest, test_anonymizer) {
  Anonymizer a{"test_id7531", [](dicom::DataElement& e) -> bool {
    if (e.tag() == dictionary::Rows::tag) return true;
    if (e.tag() == dictionary::Columns::tag) {
      e.get_manipulator<typename dictionary::Columns::manipulator_type>()->at(0) = 777;
    }
    return false;
  }};

  std::string filename = tests::path_to_file("data/undefined_SQ_rtdose.dcm");
  dicom::File file(filename);

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

  EXPECT_EQ(file.data_set()->element<dictionary::PatientBirthDate>()->manipulator()->size(), 0);
  EXPECT_EQ(file.data_set()->element<dictionary::PatientSex>()->manipulator()->size(), 0);

  EXPECT_FALSE(file.data_set()->element<dictionary::Rows>());
  EXPECT_EQ(file.data_set()->element<dictionary::Columns>()->manipulator()->at(0), 777);
}
