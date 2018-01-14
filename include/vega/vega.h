#pragma once

#include <stdexcept>
#include <string>

namespace vega {
  class Exception : public std::runtime_error {
    public:
      Exception() : std::runtime_error("")
      {}

      Exception(std::string msg) : std::runtime_error(msg)
      {}
  };

  enum class Endian {
    LITTLE,
    BIG
  };

  Endian get_machine_endian();
}
