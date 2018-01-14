#include "vega/manipulators/value_manipulator.h"

namespace vega {
  namespace manipulators {
    ValueManipulator::~ValueManipulator() {}

    bool ValueManipulator::operator==(const ValueManipulator& other) const {
      return this->str() == other.str();
    }

    bool ValueManipulator::operator!=(const ValueManipulator& other) const {
      return this->str() != other.str();
    }
  }
}
