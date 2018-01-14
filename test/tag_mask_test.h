#pragma once

#include "test_helper.h"

#include "vega/tag_mask.h"

using namespace vega;

TEST(TagMaskTest, tag_constants_test) {
  {
    TagMask mask {0x1234, 0xFFFF, 0x4444, 0xFFFF};
    EXPECT_TRUE(mask.is_single());
    EXPECT_EQ(mask.singular_tag(), Tag(0x1234, 0x4444));
    EXPECT_EQ(mask.value_tag(), Tag(0x1234, 0x4444));
    EXPECT_EQ(mask.mask_tag(), Tag(0xFFFF, 0xFFFF));

    EXPECT_TRUE(mask.contains(Tag(0x1234, 0x4444)));
    EXPECT_FALSE(mask.contains(Tag(0x1230, 0x4444)));
    EXPECT_FALSE(mask.contains(Tag(0x1234, 0x4244)));

    EXPECT_EQ(mask.mask_popcount(), 32);
  }

  {
    TagMask mask {0xab00, 0xFF00, 0x1234, 0xFFFF};
    EXPECT_FALSE(mask.is_single());
    ASSERT_THROW(mask.singular_tag(), vega::Exception);
    EXPECT_EQ(mask.value_tag(), Tag(0xab00, 0x1234));
    EXPECT_EQ(mask.mask_tag(), Tag(0xFF00, 0xFFFF));

    EXPECT_TRUE(mask.contains(Tag(0xab14, 0x1234)));
    EXPECT_TRUE(mask.contains(Tag(0xabff, 0x1234)));
    EXPECT_FALSE(mask.contains(Tag(0xac14, 0x1234)));
    EXPECT_FALSE(mask.contains(Tag(0xab00, 0x4321)));

    EXPECT_EQ(mask.mask_popcount(), 24);
  }

  {
    TagMask mask {0x0001, 0x0001, 0x0000, 0x0000};
    EXPECT_FALSE(mask.is_single());
    ASSERT_THROW(mask.singular_tag(), vega::Exception);
    EXPECT_EQ(mask.value_tag(), Tag(0x0001, 0x0000));
    EXPECT_EQ(mask.mask_tag(), Tag(0x0001, 0x0000));

    EXPECT_TRUE(mask.contains(Tag(0x7493, 0xabcd)));
    EXPECT_FALSE(mask.contains(Tag(0x7492, 0xabcd)));

    EXPECT_EQ(mask.mask_popcount(), 1);
  }
}

TEST(TagMaskTest, from_string_test) {
  {
    TagMask mask {"xx11", "abxx"};
    EXPECT_EQ(mask.value_tag(), Tag(0x0011, 0xab00));
    EXPECT_EQ(mask.mask_tag(),  Tag(0x00FF, 0xFF00));
  }
}
