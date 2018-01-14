#pragma once

#include "../test_helper.h"

#include "vega/dicom/transfer_syntax.h"

using namespace vega;

TEST(TransferSyntaxTest, transfer_syntax_test) {
  EXPECT_EQ(vega::dicom::TransferSyntax::IMPLICIT_VR_LITTLE_ENDIAN.name(), "Implicit VR Little Endian");
  EXPECT_EQ(vega::dicom::TransferSyntax::IMPLICIT_VR_LITTLE_ENDIAN.uid(), UID("1.2.840.10008.1.2"));

  EXPECT_EQ(vega::dicom::TransferSyntax::IMPLICIT_VR_LITTLE_ENDIAN.is_explicit_vr(), false);
  EXPECT_EQ(vega::dicom::TransferSyntax::IMPLICIT_VR_LITTLE_ENDIAN.endianness(), Endian::LITTLE);

  EXPECT_EQ(vega::dicom::TransferSyntax::EXPLICIT_VR_LITTLE_ENDIAN.is_explicit_vr(), true);
  EXPECT_EQ(vega::dicom::TransferSyntax::EXPLICIT_VR_LITTLE_ENDIAN.endianness(), Endian::LITTLE);

  EXPECT_EQ(vega::dicom::TransferSyntax::EXPLICIT_VR_BIG_ENDIAN.is_explicit_vr(), true);
  EXPECT_EQ(vega::dicom::TransferSyntax::EXPLICIT_VR_BIG_ENDIAN.endianness(), Endian::BIG);

  EXPECT_EQ(vega::dicom::TransferSyntax("Explicit VR Big Endian").is_explicit_vr(), true);
  EXPECT_EQ(vega::dicom::TransferSyntax("Explicit VR Big Endian").endianness(), Endian::BIG);

  EXPECT_EQ(vega::dicom::TransferSyntax("Implicit VR Little Endian").is_explicit_vr(), false);
  EXPECT_EQ(vega::dicom::TransferSyntax("Implicit VR Little Endian").endianness(), Endian::LITTLE);
}
