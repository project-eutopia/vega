#include "vega/randomizer.h"

#include <random>

namespace vega {
  Randomizer::Randomizer() : gen_(std::random_device()())
  {}
}
