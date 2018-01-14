#include "vega/multi_vr.h"
#include "vega/vega.h"

#include <algorithm>

namespace vega {
  MultiVR::MultiVR(const std::string& vrs) {
    // String should be size 3n - 1
    // e.g. "OB/OW" is size 5, with n = 2
    if (vrs.size() % 3 != 2) {
      throw InvalidMultiVR(std::string("Invalid MultiVR string \"") + vrs + std::string("\""));
    }

    size_t n = (vrs.size() + 1) / 3;

    for (unsigned i = 0; i < n; ++i) {
      m_vrs.push_back(
        vega::vr::parse_vr_string(vrs.substr(3*i, 2))
      );
    }
  }

  const std::vector<VR>& MultiVR::vrs() const { return m_vrs; }
  bool MultiVR::single() const { return m_vrs.size() == 1; }

  bool MultiVR::contains(const VR& vr) const {
    return std::find(m_vrs.begin(), m_vrs.end(), vr) != m_vrs.end();
  }

  // Possible MultiVR with more than one VR:
  // OB/OW
  // US/SS
  // US/SS/OW
  // US/OW
  const VR& MultiVR::to_single_vr() const {
    if (this->single()) return m_vrs[0];

    if (m_vrs.size() == 2) {
      if (m_vrs[0] == vr::OB && m_vrs[1] == vr::OW) {
        return vr::OX;
      }
      if (m_vrs[0] == vr::US) {
        if (m_vrs[1] == vr::SS) {
          return vr::XS;
        }
        else if (m_vrs[1] == vr::OW) {
          return vr::UW;
        }
      }
    }
    else if (m_vrs.size() == 3) {
      if (m_vrs[0] == vr::US && m_vrs[1] == vr::SS && m_vrs[2] == vr::OW) {
        return vr::XW;
      }
    }

    throw InvalidMultiVR("Invalid MultiVR");
  }

  bool MultiVR::needs_two_byte_padding() const {
    if (!this->single()) throw AmbiguousMultiVR("Cannot determine if needs two byte padding");
    return m_vrs[0].needs_two_byte_padding();
  }

  bool MultiVR::is_sequence() const {
    if (!this->single()) throw AmbiguousMultiVR("Cannot determine if is sequence");
    return m_vrs[0].is_sequence();
  }

  std::ostream& operator<<(std::ostream& os, const MultiVR& multi_vr) {
    if (multi_vr.vrs().size() > 0) {
      os << multi_vr.vrs()[0];
      for (size_t i = 1; i < multi_vr.vrs().size(); ++i) {
        os << "/" << multi_vr.vrs()[1];
      }
    }
    return os;
  }

  const MultiVR& MultiVR::from_vr(const VR& vr) {
    static const std::map<VR, MultiVR> vr_to_multi = {
      {VR("AE"), MultiVR("AE")},
      {VR("AS"), MultiVR("AS")},
      {VR("AT"), MultiVR("AT")},
      {VR("CS"), MultiVR("CS")},
      {VR("DA"), MultiVR("DA")},
      {VR("DS"), MultiVR("DS")},
      {VR("DT"), MultiVR("DT")},
      {VR("FL"), MultiVR("FL")},
      {VR("FD"), MultiVR("FD")},
      {VR("IS"), MultiVR("IS")},
      {VR("LO"), MultiVR("LO")},
      {VR("LT"), MultiVR("LT")},
      {VR("OB"), MultiVR("OB")},
      {VR("OD"), MultiVR("OD")},
      {VR("OF"), MultiVR("OF")},
      {VR("OW"), MultiVR("OW")},
      {VR("PN"), MultiVR("PN")},
      {VR("SH"), MultiVR("SH")},
      {VR("SL"), MultiVR("SL")},
      {VR("SQ"), MultiVR("SQ")},
      {VR("SS"), MultiVR("SS")},
      {VR("ST"), MultiVR("ST")},
      {VR("TM"), MultiVR("TM")},
      {VR("UC"), MultiVR("UC")},
      {VR("UI"), MultiVR("UI")},
      {VR("UL"), MultiVR("UL")},
      {VR("UN"), MultiVR("UN")},
      {VR("UR"), MultiVR("UR")},
      {VR("US"), MultiVR("US")},
      {VR("UT"), MultiVR("UT")},
      {VR("ox"), MultiVR("OB/OW")},
      {VR("xs"), MultiVR("SS/US")},
      {VR("xw"), MultiVR("SS/US/OW")},
      {VR("uw"), MultiVR("US/OW")}
    };

    auto it = vr_to_multi.find(vr);
    if (it == vr_to_multi.end()) {
      throw vega::Exception("In MultiVR::from_vr(), Could not find vr " + vr.str());
    }
    return it->second;
  }
}
