#pragma once

#include "test_helper.h"

#include "vega/string.h"

using namespace vega;

TEST(StringTest, byte_to_string_test) {
  Byte byte = {.u = 200};
  EXPECT_EQ(vega::to_string<Byte>(byte), "0xc8");
}

TEST(StringTest, byte_from_string_test) {
  Byte byte = vega::from_string<Byte>("0xFF");
  EXPECT_EQ(byte.u, 255);
}

TEST(StringTest, word_to_string_test) {
  Word word = {.u = 12832};
  EXPECT_EQ(vega::to_string<Word>(word), "0x3220");
}

TEST(StringTest, word_from_string_test) {
  Word word = vega::from_string<Word>("0x0200");
  EXPECT_EQ(word.u, 512);
}
