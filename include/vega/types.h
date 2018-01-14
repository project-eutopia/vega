#pragma once

#include <stdint.h>
#include <ostream>

namespace vega {
  union Byte {
    uint8_t u;
    int8_t i;
    char c;
    // TODO: implicit constructors?

    bool operator==(const Byte& other) const;
    bool operator!=(const Byte& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Byte& byte);
  };

  union Word {
    uint16_t u;
    int16_t i;

    bool operator==(const Word& other) const;
    bool operator!=(const Word& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Word& word);
  };

  union Long {
    uint32_t u;
    int32_t i;

    bool operator==(const Long& other) const;
    bool operator!=(const Long& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Long& l);
  };
}
