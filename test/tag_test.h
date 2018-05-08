#pragma once

#include "test_helper.h"

#include "vega/tag.h"

using namespace vega;

TEST(TagTest, tag_constants_test) {
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

TEST(TagTest, tag_stream_operators_test) {
  Tag tag{0x1234,0xff00};

  std::stringstream ss;
  ss << tag;
  EXPECT_EQ(ss.str(), "(1234,FF00)");

  ss = std::stringstream("(ABCD,0010)");
  ss >> tag;
  EXPECT_EQ(tag.group(), 0xabcd);
  EXPECT_EQ(tag.element(), 0x0010);
}
