#pragma once

#include <string>
#include <iostream>

namespace vega {
  class UID {
    public:
      static const UID IMPLEMENTATION_CLASS_UID;

      UID();
      explicit UID(const std::string& uid);

      // Supply a number to be included in the UID (for instance you might
      // want to include the CT slice number in the UID)
      // Value of zero is ignored
      static UID generate(uint16_t num = 0);

      const std::string& str() const;
      bool operator==(const UID& other) const;
      bool operator!=(const UID& other) const;
      bool operator<(const UID& other) const;

      friend std::ostream& operator<<(std::ostream& os, const UID& uid);
      friend std::istream& operator>>(std::istream& is, UID& uid);

    private:
      static const UID BASE;
      static const size_t MAX_LENGTH;

      std::string m_uid;

      static std::string timestamp();
  };
}
