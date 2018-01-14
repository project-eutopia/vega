#include "vega/types.h"
#include "vega/string.h"

namespace vega {
  bool Byte::operator==(const Byte& other) const {
    return this->u == other.u;
  }

  bool Byte::operator!=(const Byte& other) const {
    return this->u != other.u;
  }

  std::ostream& operator<<(std::ostream& os, const Byte& byte) {
    os << vega::to_string(byte);
    return os;
  }

  bool Word::operator==(const Word& other) const {
    return this->u == other.u;
  }

  bool Word::operator!=(const Word& other) const {
    return this->u != other.u;
  }

  std::ostream& operator<<(std::ostream& os, const Word& word) {
    os << vega::to_string(word);
    return os;
  }

  bool Long::operator==(const Long& other) const {
    return this->u == other.u;
  }

  bool Long::operator!=(const Long& other) const {
    return this->u != other.u;
  }

  std::ostream& operator<<(std::ostream& os, const Long& l) {
    os << vega::to_string(l);
    return os;
  }
}
