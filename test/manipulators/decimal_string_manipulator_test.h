#pragma once

#include "../test_helper.h"

#include <iostream>
#include <sstream>
#include <random>

#include "vega/manipulators/decimal_string_manipulator.h"
#include "vega/dicom/data_element.h"
#include "vega/math.h"

using namespace vega;
using namespace vega::manipulators;

TEST(DecimalStringTest, decimal_string_test) {
  std::default_random_engine generator;
  std::uniform_real_distribution<double> base_distribution(1.0, 10.0);
  std::uniform_real_distribution<double> exp_distribution(-307, 307);
  std::uniform_real_distribution<double> factor_distribution(-1, 1);

  double base, exp, value;
  for (size_t i = 0; i < 2000; ++i) {
    base = base_distribution(generator);
    exp  = exp_distribution(generator);
    value = factor_distribution(generator) * std::pow(base, exp);

    DecimalString ds{value};
    // Must fit in 16 characters
    EXPECT_LE(ds.str().size(), 16);

    DecimalString ds2{ds.str()};

    if (value == 0) {
      EXPECT_EQ(double(ds), 0);
    }
    else if (value > 0) {
      EXPECT_GT(double(ds2), value*0.99999);
      EXPECT_LT(double(ds2), value*1.00001);
    }
    else {
      EXPECT_LT(double(ds2), value*0.99999);
      EXPECT_GT(double(ds2), value*1.00001);
    }
  }
}

TEST(DecimalStringTest, decimal_string_stream_test) {
  DecimalString ds;

  std::stringstream ss("9.9902680e-1");
  ss >> ds;

  EXPECT_EQ(double(ds), 0.9990268);

  ss = std::stringstream();
  ss << ds;

  EXPECT_EQ(ss.str(), "9.990268e-1");

  DecimalStringManipulator manipulator{};
  manipulator.emplace_back(0.9990268);
}

TEST(DecimalStringManipulatorTest, constructor_test) {
  DecimalStringManipulator manipulator{};

  manipulator.emplace_back(-3.14e13);
  manipulator.emplace_back(123.456789012346);

  EXPECT_EQ(manipulator.str(), "-3.14e13\\123.45678901235");

  DecimalStringManipulator manipulator2(manipulator.raw_value());

  EXPECT_TRUE(manipulator[0] == manipulator2[0]);
  EXPECT_NEAR(manipulator[1], manipulator2[1], 1e-10);
}

TEST(DecimalStringManipulatorTest, data_element_test) {
  vega::dicom::DataElement data_element{Tag {0x0010, 0x1020}, vr::DS};
  auto manipulator = data_element.get_manipulator<DecimalStringManipulator>();

  manipulator->push_back(-1.25e-100);
  manipulator->push_back(5.67890123456789e210);

  EXPECT_EQ(data_element.str(), "-1.25e-100\\5.6789012346e210 ");
}
