#pragma once

#include "test_helper.h"

#include "vega/undefined_length_remover.h"

using namespace vega;

TEST(UndefinedLengthRemoverTest, test_undefined_length_remover) {
  UndefinedLengthRemover remover{};

  std::string filename = tests::path_to_file("data/undefined_SQ_rtdose.dcm");
  dicom::File file_original(filename);

  dicom::File file_modified(filename);
  remover.remove_undefined_lengths(file_modified);

  std::shared_ptr<std::stringstream> ss = std::make_shared<std::stringstream>();
  file_modified.write(ss);
  dicom::File file_copy(ss);

  // Content unchanged
  EXPECT_EQ(*file_original.data_set(), *file_copy.data_set());

  /* (300C,0002) SQ "ReferencedRTPlanSequence" VM=1 (len=4294967295): */
  /*   --- Data set 1/1 with 3 elements --- */
  /*   (0008,1150) UI "ReferencedSOPClassUID" VM=1 (len=30): 1.2.840.10008.5.1.4.1.1.481.5 */
  /*   (0008,1155) UI "ReferencedSOPInstanceUID" VM=1 (len=40): 1.2.3.4.5.6.7.8.9.0.1.2.3.4.5.6.7.8.9.4 */
  /*   (300C,0020) SQ "ReferencedFractionGroupSequence" VM=1 (len=4294967295): */
  /*     --- Data set 1/1 with 2 elements --- */
  /*     (300C,0004) SQ "ReferencedBeamSequence" VM=1 (len=4294967295): */
  /*       --- Data set 1/1 with 1 elements --- */
  /*       (300C,0006) IS "ReferencedBeamNumber" VM=1 (len=2): 2 */
  /*     (300C,0022) IS "ReferencedFractionGroupNumber" VM=1 (len=2): 1 */
  /* (7FE0,0010) OW "PixelData" VM=1 (len=23217): Pixel Data (size 23217) */

  EXPECT_EQ(file_original.data_set()->element<dictionary::ReferencedRTPlanSequence>()->length(), dicom::DataElementHeader::UNDEFINED_LENGTH);
  EXPECT_NE(file_modified.data_set()->element<dictionary::ReferencedRTPlanSequence>()->length(), dicom::DataElementHeader::UNDEFINED_LENGTH);

  EXPECT_EQ(
      file_original.data_set()->element<dictionary::ReferencedRTPlanSequence>()->data_sets()[0]->element<dictionary::ReferencedFractionGroupSequence>()->length(),
    dicom::DataElementHeader::UNDEFINED_LENGTH
  );
  EXPECT_NE(
      file_modified.data_set()->element<dictionary::ReferencedRTPlanSequence>()->data_sets()[0]->element<dictionary::ReferencedFractionGroupSequence>()->length(),
    dicom::DataElementHeader::UNDEFINED_LENGTH
  );

  EXPECT_EQ(
      file_original.data_set()->element<dictionary::ReferencedRTPlanSequence>()->data_sets()[0]->element<dictionary::ReferencedFractionGroupSequence>()->data_sets()[0]->element<dictionary::ReferencedBeamSequence>()->length(),
    dicom::DataElementHeader::UNDEFINED_LENGTH
  );
  EXPECT_NE(
      file_modified.data_set()->element<dictionary::ReferencedRTPlanSequence>()->data_sets()[0]->element<dictionary::ReferencedFractionGroupSequence>()->data_sets()[0]->element<dictionary::ReferencedBeamSequence>()->length(),
    dicom::DataElementHeader::UNDEFINED_LENGTH
  );
}
