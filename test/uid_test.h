#pragma once

#include "test_helper.h"

#include "vega/uid.h"

using namespace vega;

TEST(UIDTest, uid_implementation_class_uid_test) {
  EXPECT_EQ(UID::IMPLEMENTATION_CLASS_UID.str(), "1.2.826.0.1.3680043.9.7229.1");
}

TEST(UIDTest, uid_length_test) {
  {
    UID uid = UID::generate();
    EXPECT_EQ(uid.str().size(), 64);
  }
  {
    UID uid = UID::generate(1);
    EXPECT_EQ(uid.str().size(), 64);
  }
  {
    UID uid = UID::generate(9);
    EXPECT_EQ(uid.str().size(), 64);
  }
  {
    UID uid = UID::generate(10);
    EXPECT_EQ(uid.str().size(), 64);
  }
  {
    UID uid = UID::generate(99);
    EXPECT_EQ(uid.str().size(), 64);
  }
  {
    UID uid = UID::generate(100);
    EXPECT_EQ(uid.str().size(), 64);
  }
  {
    UID uid = UID::generate(999);
    EXPECT_EQ(uid.str().size(), 64);
  }
  {
    UID uid = UID::generate(1000);
    EXPECT_EQ(uid.str().size(), 64);
  }
  {
    UID uid = UID::generate(9999);
    EXPECT_EQ(uid.str().size(), 64);
  }
  {
    UID uid = UID::generate(10000);
    EXPECT_EQ(uid.str().size(), 64);
  }
  {
    UID uid = UID::generate(65535);
    EXPECT_EQ(uid.str().size(), 64);
  }
}
