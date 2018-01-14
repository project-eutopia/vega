#pragma once

#include <vector>
#include <regex>
#include <string>

#include "vega/vega.h"
#include "vega/vr.h"

namespace vega {
  class MultiVR {
    private:
      std::vector<VR> m_vrs;

    public:
      class InvalidMultiVR : public vega::Exception { using vega::Exception::Exception; };
      class AmbiguousMultiVR : public vega::Exception { using vega::Exception::Exception; };

      explicit MultiVR(const std::string& vrs);

      const std::vector<VR>& vrs() const;
      bool single() const;

      bool contains(const VR& vr) const;

      const VR& to_single_vr() const;

      bool needs_two_byte_padding() const;
      bool is_sequence() const;

      static const MultiVR& from_vr(const VR& vr);

      friend std::ostream& operator<<(std::ostream& os, const MultiVR& multi_vr);
  };
}
