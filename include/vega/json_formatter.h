#pragma once

#include <sstream>
#include <ostream>

namespace vega {
  typename <typename T>
  class JsonFormatter {
    private:
      const T& m_ref;

    public:
      JsonFormatter(const T& t) : m_ref(t) {}

      std::string str() const {
        std::stringstream ss;
        ss << *this;
        return ss.str();
      }

      friend std::ostream& operator<<(std::ostream& os, const JsonFormatter& f) {
        os << f.m_ref;
        return os;
      }
  };

  template <typename T>
  const JsonFormatter<T> to_json_formatter(const T& t) {
    return JsonFormatter<T>(t);
  }
}
