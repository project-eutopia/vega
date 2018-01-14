#pragma once

#include "test_helper.h"

#include "vega/multi_vr.h"

using namespace vega;

TEST(MultiVRTest, multi_vr_test) {
  {
    ASSERT_THROW(MultiVR(""), MultiVR::InvalidMultiVR);
  }
  {
    const MultiVR multi_vr("OB");
    EXPECT_EQ(multi_vr.vrs().size(), 1);
    EXPECT_EQ(multi_vr.vrs()[0], vr::OB);
  }
  {
    const MultiVR multi_vr("OB/OW");
    EXPECT_EQ(multi_vr.vrs().size(), 2);
    EXPECT_EQ(multi_vr.vrs()[0], vr::OB);
    EXPECT_EQ(multi_vr.vrs()[1], vr::OW);
  }
  {
    const MultiVR multi_vr("US/SS/OW");
    EXPECT_EQ(multi_vr.vrs().size(), 3);
    EXPECT_EQ(multi_vr.vrs()[0], vr::US);
    EXPECT_EQ(multi_vr.vrs()[1], vr::SS);
    EXPECT_EQ(multi_vr.vrs()[2], vr::OW);

    EXPECT_TRUE(multi_vr.contains(vr::US));
    EXPECT_FALSE(multi_vr.contains(vr::AE));
  }
}
