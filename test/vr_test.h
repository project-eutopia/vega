#pragma once

#include "test_helper.h"

#include "vega/vr.h"

using namespace vega;

TEST(VRTest, vr_test) {
  {
    const VR vr = vr::parse_vr_string(std::string("AE"));
    EXPECT_EQ(vr.data().value, 0x4541);
    EXPECT_EQ(vr, vr::AE);
    EXPECT_EQ(vr.name(), std::string("AE"));
  }
}

TEST(VRTest, is_sequence_test) {
  EXPECT_TRUE(vr::SQ.is_sequence());
  EXPECT_FALSE(vr::AE.is_sequence());
}

TEST(VRTest, is_combined_vr) {
  EXPECT_FALSE(vr::UN.is_combined_vr());
  EXPECT_TRUE(vr::OX.is_combined_vr());
  EXPECT_TRUE(vr::XS.is_combined_vr());
  EXPECT_TRUE(vr::XW.is_combined_vr());
  EXPECT_TRUE(vr::UW.is_combined_vr());
}

TEST(VRTest, vr_constants_test) {
  EXPECT_EQ(vr::AE.name(), std::string("AE"));
  EXPECT_EQ(vr::AS.name(), std::string("AS"));
  EXPECT_EQ(vr::AT.name(), std::string("AT"));
  EXPECT_EQ(vr::CS.name(), std::string("CS"));
  EXPECT_EQ(vr::DA.name(), std::string("DA"));
  EXPECT_EQ(vr::DS.name(), std::string("DS"));
  EXPECT_EQ(vr::DT.name(), std::string("DT"));
  EXPECT_EQ(vr::FD.name(), std::string("FD"));
  EXPECT_EQ(vr::FL.name(), std::string("FL"));
  EXPECT_EQ(vr::IS.name(), std::string("IS"));
  EXPECT_EQ(vr::LO.name(), std::string("LO"));
  EXPECT_EQ(vr::LT.name(), std::string("LT"));
  EXPECT_EQ(vr::OB.name(), std::string("OB"));
  EXPECT_EQ(vr::OD.name(), std::string("OD"));
  EXPECT_EQ(vr::OF.name(), std::string("OF"));
  EXPECT_EQ(vr::OW.name(), std::string("OW"));
  EXPECT_EQ(vr::PN.name(), std::string("PN"));
  EXPECT_EQ(vr::SH.name(), std::string("SH"));
  EXPECT_EQ(vr::SL.name(), std::string("SL"));
  EXPECT_EQ(vr::SQ.name(), std::string("SQ"));
  EXPECT_EQ(vr::SS.name(), std::string("SS"));
  EXPECT_EQ(vr::ST.name(), std::string("ST"));
  EXPECT_EQ(vr::TM.name(), std::string("TM"));
  EXPECT_EQ(vr::UI.name(), std::string("UI"));
  EXPECT_EQ(vr::UL.name(), std::string("UL"));
  EXPECT_EQ(vr::UN.name(), std::string("UN"));
  EXPECT_EQ(vr::US.name(), std::string("US"));
  EXPECT_EQ(vr::UT.name(), std::string("UT"));
  EXPECT_EQ(vr::OX.name(), std::string("ox"));
  EXPECT_EQ(vr::XS.name(), std::string("xs"));
  EXPECT_EQ(vr::XW.name(), std::string("xw"));
  EXPECT_EQ(vr::UW.name(), std::string("uw"));
}
