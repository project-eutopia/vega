#pragma once

#include <stdint.h>
#include <iostream>

namespace vega {
  union Byte {
    uint8_t u;
    int8_t i;
    char c;
    // TODO: implicit constructors?

    bool operator==(const Byte& other) const;
    bool operator!=(const Byte& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Byte& byte);
    friend std::istream& operator>>(std::istream& in, Byte& byte);
  };

  union Word {
    uint16_t u;
    int16_t i;

    bool operator==(const Word& other) const;
    bool operator!=(const Word& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Word& word);
    friend std::istream& operator>>(std::istream& in, Word& word);
  };

  union Long {
    uint32_t u;
    int32_t i;

    bool operator==(const Long& other) const;
    bool operator!=(const Long& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Long& l);
    friend std::istream& operator>>(std::istream& is, Long& l);
  };

  class DecimalString {
    private:
      double m_double;

    public:
      explicit DecimalString();

      // Allow conversion from arbitrary type if allowed
      template <typename T>
      DecimalString(const T& t) : m_double(t) {}
      DecimalString(const std::string& s);

      operator double() const;
      operator float() const;

      bool operator ==(const DecimalString& ds) const;
      bool operator !=(const DecimalString& ds) const;

      std::string str() const;

      friend std::ostream& operator<<(std::ostream& os, const DecimalString& ds);
      friend std::istream& operator>>(std::istream& is, DecimalString& ds);
  };
}
