#include "vega/vr.h"
#include "vega/types.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <unordered_map>

namespace vega {
  bool VR::operator==(const VR& other) const {
    return this->data().value == other.data().value;
  }

  bool VR::operator!=(const VR& other) const {
    return !(*this == other);
  }

  bool VR::operator<(const VR& other) const {
    return this->data().value < other.data().value;
  }

  std::string VR::name() const {
    return std::string("") + data().characters[0] + data().characters[1];
  }

  std::string VR::str() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
  }

  std::string VR::long_str() const {
    std::stringstream ss;
    ss << (*this);
    ss << " (" << Word{this->data().value} << ")";
    return ss.str();
  }

  size_t VR::block_size() const {
    static const std::unordered_map<value_type,size_t> BYTE_SIZES = {

      // 1 byte read/write
      {vr::AE_VALUE, 1},
      {vr::AS_VALUE, 1},
      {vr::AE_VALUE, 1},
      {vr::AS_VALUE, 1},
      {vr::CS_VALUE, 1},
      {vr::DA_VALUE, 1},
      {vr::DS_VALUE, 1},
      {vr::DT_VALUE, 1},
      {vr::IS_VALUE, 1},
      {vr::LO_VALUE, 1},
      {vr::LT_VALUE, 1},
      {vr::OB_VALUE, 1},
      {vr::PN_VALUE, 1},
      {vr::SH_VALUE, 1},
      {vr::ST_VALUE, 1},
      {vr::TM_VALUE, 1},
      {vr::UC_VALUE, 1},
      {vr::UI_VALUE, 1},
      {vr::UT_VALUE, 1},
      {vr::UR_VALUE, 1},
      // Not-applicable, won't be used
      {vr::SQ_VALUE, 1},
      // Unknown
      {vr::UN_VALUE, 1},

      // 2 byte read/write
      {vr::AT_VALUE, 2},
      {vr::OW_VALUE, 2},
      {vr::SS_VALUE, 2},
      {vr::US_VALUE, 2},
      // Mixed types
      {vr::XS_VALUE, 2},
      {vr::XW_VALUE, 2},
      {vr::UW_VALUE, 2},

      // 4 byte read/write
      {vr::FL_VALUE, 4},
      {vr::OF_VALUE, 4},
      {vr::SL_VALUE, 4},
      {vr::UL_VALUE, 4},
      {vr::OL_VALUE, 4},

      // 4 byte read/write
      {vr::FD_VALUE, 8},
      {vr::OD_VALUE, 8},

      // Ambiguous mixed type
      {vr::OX_VALUE, 0}
    };

    // Hard fail if unknown VR is used
    return BYTE_SIZES.at(m_data.value);
  }

  bool VR::valid() const {
    switch (this->data().characters[0]) {
      case 'A':
        switch (this->data().characters[1]) {
          case 'E':
          case 'S':
          case 'T':
            return true;
          default:
            return false;
        }

      case 'C':
        return this->data().characters[1] == 'S';

      case 'D':
        switch (this->data().characters[1]) {
          case 'A':
          case 'S':
          case 'T':
            return true;
          default:
            return false;
        }

      case 'F':
        switch (this->data().characters[1]) {
          case 'L':
          case 'D':
            return true;
          default:
            return false;
        }

      case 'I':
        return this->data().characters[1] == 'S';

      case 'L':
        switch (this->data().characters[1]) {
          case 'O':
          case 'T':
            return true;
          default:
            return false;
        }

      case 'O':
        switch (this->data().characters[1]) {
          case 'B':
          case 'D':
          case 'F':
          case 'L':
          case 'W':
            return true;
          default:
            return false;
        }

      case 'P':
        return this->data().characters[1] == 'N';

      case 'S':
        switch (this->data().characters[1]) {
          case 'H':
          case 'L':
          case 'Q':
          case 'S':
          case 'T':
            return true;
          default:
            return false;
        }

      case 'T':
        return this->data().characters[1] == 'M';

      case 'U':
        switch (this->data().characters[1]) {
          case 'C':
          case 'I':
          case 'L':
          case 'N':
          case 'R':
          case 'S':
          case 'T':
            return true;
          default:
            return false;
        }

      case 'o':
        return this->data().characters[1] == 'x';

      case 'x':
        switch (this->data().characters[1]) {
          case 's':
          case 'w':
            return true;
          default:
            return false;
        }

      case 'u':
        return this->data().characters[1] == 'w';

      default:
        return false;
    }
  }



  bool VR::needs_two_byte_padding() const {
    /* True if: "OB", "OD", "OF", "OL", "OW", "SQ", "UC", "UR", "UT" or "UN" */
    // TODO: handle lowercase multiple VRs, like "ox"
    switch (this->data().characters[0]) {
      case 'O':
        switch (this->data().characters[1]) {
          case 'B':
          case 'D':
          case 'F':
          case 'L':
          case 'W':
            return true;
          default:
            return false;
        }
      case 'S':
        return this->data().characters[1] == 'Q';
      case 'U':
        switch (this->data().characters[1]) {
          case 'C':
          case 'R':
          case 'T':
          case 'N':
            return true;
          default:
            return false;
        }
      default:
        return false;
    }
  }

  bool VR::is_sequence() const {
    return this->data().value == vr::SQ_VALUE;
  }

  bool VR::is_combined_vr() const {
    return
      this->data().value == vr::OX_VALUE ||
      this->data().value == vr::XS_VALUE ||
      this->data().value == vr::XW_VALUE ||
      this->data().value == vr::UW_VALUE;
  }

  namespace vr {
    VR parse_vr_string(std::string s) {
      assert(s.length() == 2);
      return VR{ VR::Data{.characters = {s[0], s[1]}} };
    }
  }

  std::ostream& operator<<(std::ostream& os, const VR& vr) {
    os << ((std::isalnum(vr.data().characters[0])) ? vr.data().characters[0] : '_');
    os << ((std::isalnum(vr.data().characters[1])) ? vr.data().characters[1] : '_');
    return os;
  }

  std::istream& operator>>(std::istream& is, VR& vr) {
    is >> vr.data().characters[0];
    is >> vr.data().characters[1];
    return is;
  }
}
