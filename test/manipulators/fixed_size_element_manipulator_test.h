#pragma once

#include "../test_helper.h"

#include <sstream>
#include "vega/manipulators/fixed_size_element_manipulator.h"
#include "vega/tag.h"
#include "vega/vr.h"

using namespace vega;
using namespace vega::manipulators;

TEST(FixedSizeElementManipulatorTest, float_test) {
  FixedSizeElementManipulator<float> manipulator{};

  manipulator.push_back(1.0f);
  manipulator.push_back(-3.14f);

  std::string s = manipulator.str();
  std::istringstream ss(s);
  std::string element_string;

  std::vector<float> elements;
  while(std::getline(ss, element_string, '\\')) {
    elements.push_back(std::stof(element_string));
  }

  EXPECT_EQ(elements[0], 1.0f);
  EXPECT_EQ(elements[1], -3.14f);

  FixedSizeElementManipulator<float> manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator2[0], 1.0f);
  EXPECT_EQ(manipulator2[1], -3.14f);
}

TEST(FixedSizeElementManipulatorTest, tag_test) {
  FixedSizeElementManipulator<Tag> manipulator{};

  manipulator.emplace_back(16, 32);
  manipulator.emplace_back(0x3000, 0xA080);

  EXPECT_EQ(manipulator.str(), "(0010,0020)\\(3000,A080)");

  FixedSizeElementManipulator<Tag> manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
  EXPECT_NE(&manipulator[0], &manipulator2[0]);
}

TEST(FixedSizeElementManipulatorTest, vr_test) {
  FixedSizeElementManipulator<VR> manipulator{};

  manipulator.emplace_back(VR::Data{.characters = {'A', 'E'}});
  manipulator.emplace_back(VR::Data{.characters = {'S', 'Q'}});

  EXPECT_EQ(manipulator.str(), "AE\\SQ");

  FixedSizeElementManipulator<VR> manipulator2(manipulator.raw_value());

  EXPECT_EQ(manipulator[0], manipulator2[0]);
  EXPECT_EQ(manipulator[1], manipulator2[1]);
}
