#pragma once

#include "test_helper.h"

#include "vega/randomizer.h"

using namespace vega;

TEST(RandomizerTest, test_randomizer) {
  Randomizer r;

  r.generate<uint8_t>();
  r.generate<uint16_t>();
  r.generate<uint32_t>();
  r.generate<uint64_t>();

  r.generate<int8_t>();
  r.generate<int16_t>();
  r.generate<int32_t>();
  r.generate<int64_t>();

  r.generate<float>();
  r.generate<double>();

  const auto s = r.generate<std::string>();
  for (auto c : s) {
    EXPECT_TRUE(c >= 'a' && c <= 'z');
  }
}
