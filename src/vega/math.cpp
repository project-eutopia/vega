#include "vega/math.h"
#include <cmath>

namespace vega {
  namespace math {
    // Credit to: https://stackoverflow.com/questions/824118/why-is-floor-so-slow
    int floori(double x) {
      return (int) x - (x < (int) x);
    }

    double floorf(double x) {
      return (double) ((int)x - (x < (int) x));
    }

    int ceili(double x) {
      return (int) x + (x > (int) x);
    }

    double ceilf(double x) {
      return (double) ((int)x + (x > (int) x));
    }

    bool isinteger(double x) {
      return floorf(x) == x;
    }

    double frexp10(double arg, int * exp) {
      if (arg == 0) {
        *exp = 0;
        return 0;
      }
      else {
        *exp = (arg == 0) ? 0 : floori(std::log10(std::fabs(arg)));
        return arg * pow(10 , -(*exp));
      }
    }

    float frexp10(float arg, int * exp) {
      return (frexp10(double(arg), exp));
    }

    unsigned popcount(uint64_t x) {
      x -= (x >> 1) & 0x5555555555555555;
      x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
      x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
      return (x * 0x0101010101010101) >> 56;
    }
  }
}
