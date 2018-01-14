#pragma once

#include "../test_helper.h"

#include "vega/dicom/sop_class.h"

using namespace vega;

TEST(SOPClassTest, sop_class_test) {
  dicom::SOPClass sop1{"RT Plan Storage"};
  EXPECT_EQ(sop1.name(), "RT Plan Storage");
  EXPECT_EQ(sop1.uid(), UID("1.2.840.10008.5.1.4.1.1.481.5"));

  dicom::SOPClass sop2{"Enhanced XRF Image Storage"};
  EXPECT_EQ(sop2.name(), "Enhanced XRF Image Storage");
  EXPECT_EQ(sop2.uid(), UID("1.2.840.10008.5.1.4.1.1.12.2.1"));

  // With UID as string
  dicom::SOPClass sop3{"1.2.840.10008.5.1.4.1.1.2"};
  EXPECT_EQ(sop3.name(), "CT Image Storage");
  EXPECT_EQ(sop3.uid(), UID("1.2.840.10008.5.1.4.1.1.2"));

  // With UID string
  dicom::SOPClass sop4{UID{"1.2.840.10008.5.1.4.1.1.4"}};
  EXPECT_EQ(sop4.name(), "MR Image Storage");
  EXPECT_EQ(sop4.uid(), UID("1.2.840.10008.5.1.4.1.1.4"));

  try {
    dicom::SOPClass invalid_sop{"Invalid Storage"};
    EXPECT_TRUE(false);
  } catch(const vega::Exception& ex) {
    EXPECT_EQ(std::string(ex.what()), std::string("Could not find SOP class with name: Invalid Storage"));
  }
}
