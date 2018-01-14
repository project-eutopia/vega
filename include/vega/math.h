#pragma once

#include <stdint.h>

namespace vega {
  namespace math {
    int    floori(double    x);
    double floorf(double    x);
    int    ceili(double     x);
    double ceilf(double     x);
    bool   isinteger(double x);

    double frexp10(double arg, int * exp);
    float frexp10(float arg, int * exp);

    unsigned popcount(uint64_t x);
  }
}
