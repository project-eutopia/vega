#include <sstream>

namespace vega {
  template <typename T>
  std::string to_string(const T& t) {
    return t.str();
  }

  template <typename T>
  std::string to_json(const T& t) {
    std::stringstream ss;
    ss << '"' << t.str() << '"';
    return ss.str();
  }

  template <typename T>
  T from_string(const std::string& s) {
    return T{s};
  }
}
