#pragma once

#include <stdexcept>
#include <string>

namespace vega {
  /**
   * \class Exception
   * \brief The base class for exceptions that are raised by the vega library.
   */
  class Exception : public std::runtime_error {
    public:
      Exception() : std::runtime_error("")
      {}

      Exception(std::string msg) : std::runtime_error(msg)
      {}
  };

  /**
   * \enum Endian
   * enum class for endianness
   */
  enum class Endian {
    LITTLE, ///< Little endian
    BIG     ///< Big endian
  };

  /// \return The \link Endian Endianness \endlink of the current machine.
  Endian get_machine_endian();
}
