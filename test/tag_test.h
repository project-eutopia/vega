#pragma once

#include "test_helper.h"

#include "vega/tag.h"

using namespace vega;

TEST(TAGTest, tag_constants_test) {
  Tag item_tag(Tag::ITEM_TAG);
  EXPECT_EQ(item_tag.group(), 0xFFFE);
  EXPECT_EQ(item_tag.element(), 0xE000);
  EXPECT_TRUE(item_tag.is_item_tag());
  EXPECT_FALSE(item_tag.is_item_delimitation_tag());
  EXPECT_FALSE(item_tag.is_sequence_delimitation_tag());

  Tag item_delim_tag(Tag::ITEM_DELIMITATION_TAG);
  EXPECT_EQ(item_delim_tag.group(), 0xFFFE);
  EXPECT_EQ(item_delim_tag.element(), 0xE00D);
  EXPECT_FALSE(item_delim_tag.is_item_tag());
  EXPECT_TRUE(item_delim_tag.is_item_delimitation_tag());
  EXPECT_FALSE(item_delim_tag.is_sequence_delimitation_tag());

  Tag seq_delim_tag(Tag::SEQ_DELIMITATION_TAG);
  EXPECT_EQ(seq_delim_tag.group(), 0xFFFE);
  EXPECT_EQ(seq_delim_tag.element(), 0xE0DD);
  EXPECT_FALSE(seq_delim_tag.is_item_tag());
  EXPECT_FALSE(seq_delim_tag.is_item_delimitation_tag());
  EXPECT_TRUE(seq_delim_tag.is_sequence_delimitation_tag());
}
