#pragma once

#include "test_helper.h"
#include "vega/math.h"

using namespace vega;

TEST(MathTest, floorf_test) {
  EXPECT_EQ(math::floorf(0.5),     0.0);
  EXPECT_EQ(math::floorf(0.99999), 0.0);
  EXPECT_EQ(math::floorf(1.0),     1.0);
  EXPECT_EQ(math::floorf(1.00001), 1.0);
  EXPECT_EQ(math::floorf(-0.5),   -1.0);
}

TEST(MathTest, floori_test) {
  EXPECT_EQ(math::floori(0.5),     0);
  EXPECT_EQ(math::floori(0.99999), 0);
  EXPECT_EQ(math::floori(1.0),     1);
  EXPECT_EQ(math::floori(1.00001), 1);
  EXPECT_EQ(math::floori(-0.5),   -1);
}

TEST(MathTest, ceilf_test) {
  EXPECT_EQ(math::ceilf(0.5),     1.0);
  EXPECT_EQ(math::ceilf(0.99999), 1.0);
  EXPECT_EQ(math::ceilf(1.0),     1.0);
  EXPECT_EQ(math::ceilf(1.00001), 2.0);
  EXPECT_EQ(math::ceilf(-0.5),    0.0);
}

TEST(MathTest, ceili_test) {
  EXPECT_EQ(math::ceili(0.5),     1);
  EXPECT_EQ(math::ceili(0.99999), 1);
  EXPECT_EQ(math::ceili(1.0),     1);
  EXPECT_EQ(math::ceili(1.00001), 2);
  EXPECT_EQ(math::ceili(-0.5),    0);
}

TEST(MathTest, isinteger_test) {
  EXPECT_TRUE(math::isinteger(-1.0));
  EXPECT_TRUE(math::isinteger(0.0));
  EXPECT_TRUE(math::isinteger(1.0));
  EXPECT_FALSE(math::isinteger(0.99999));
  EXPECT_FALSE(math::isinteger(1.00001));
}

TEST(MathTest, frexp10_test) {
  int exponent;
  double mantissa;

  mantissa = math::frexp10(1.0, &exponent);
  EXPECT_NEAR(mantissa, 1.0, 1e-10);
  EXPECT_EQ(exponent, 0);

  mantissa = math::frexp10(5.0, &exponent);
  EXPECT_NEAR(mantissa, 5.0, 1e-10);
  EXPECT_EQ(exponent, 0);

  mantissa = math::frexp10(-7.0, &exponent);
  EXPECT_NEAR(mantissa, -7.0, 1e-10);
  EXPECT_EQ(exponent, 0);

  mantissa = math::frexp10(789.0, &exponent);
  EXPECT_NEAR(mantissa, 7.89, 1e-10);
  EXPECT_EQ(exponent, 2);

  mantissa = math::frexp10(1.23e99, &exponent);
  EXPECT_NEAR(mantissa, 1.23, 1e-10);
  EXPECT_EQ(exponent, 99);

  mantissa = math::frexp10(-3.33e-111, &exponent);
  EXPECT_NEAR(mantissa, -3.33, 1e-10);
  EXPECT_EQ(exponent, -111);

  mantissa = math::frexp10(1.46295e-71, &exponent);
  EXPECT_NEAR(mantissa, 1.46295, 1e-10);
  EXPECT_EQ(exponent, -71);
}

TEST(MathTest, popcount_test) {
  EXPECT_EQ(math::popcount(0), 0);

  EXPECT_EQ(math::popcount(1), 1);
  EXPECT_EQ(math::popcount(4), 1);
  EXPECT_EQ(math::popcount(65536), 1);

  EXPECT_EQ(math::popcount(3), 2);
  EXPECT_EQ(math::popcount(65537), 2);

  EXPECT_EQ(math::popcount(15), 4);
  EXPECT_EQ(math::popcount(510), 8);
}
