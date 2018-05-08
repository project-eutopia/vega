#pragma once

#include "vega/vega.h"

#include <typeinfo>
#include <stdint.h>
#include <string>
#include <iostream>

namespace vega {
  /**
   * See http://dicom.nema.org/dicom/2013/output/chtml/part05/sect_6.2.html
   */
  class VR {
    public:
      template <typename T>
      class InvalidValueManipulator : public vega::Exception {
        public:
          InvalidValueManipulator(const VR& vr)
            : vega::Exception(
                std::string("Invalid type ") +
                typeid(T).name() +
                std::string(" for VR ") +
                vr.str()
              )
          {
          }
      };

      typedef uint16_t value_type;

      union Data {
        value_type value;
        char characters[2];
      };

    private:
      Data m_data;

    public:
      constexpr VR() : m_data() {}
      explicit constexpr VR(Data data) : m_data(data) {}
      explicit constexpr VR(value_type value) : m_data{ .value = value } {}
      explicit constexpr VR(char c1, char c2) : m_data{ .characters = {c1, c2} } {}
      explicit VR(const std::string& chars) : m_data{ .characters = {chars[0], chars[1]} } {}

      const Data& data() const { return m_data; }
      Data& data() { return m_data; }
      bool operator==(const VR& other) const;
      bool operator!=(const VR& other) const;
      bool operator<(const VR& other) const;

      std::string name() const;

      std::string str() const;
      std::string long_str() const;

      bool valid() const;
      bool needs_two_byte_padding() const;
      bool is_sequence() const;
      // True if type ox, xs, xw, uw, those which stand for multiple actual VRs
      bool is_combined_vr() const;

      // Returns 0 if ambiguous/unknown
      size_t block_size() const;

      friend std::ostream& operator<<(std::ostream& os, const VR& tag);
      friend std::istream& operator>>(std::istream& is, VR& tag);

      template <typename T>
      void validate_value_manipulator() const;
  };

  namespace vr {
    // Manipulators are invalid in general, specifics cases will override to allow
    template <typename T>
    bool manipulator_is_valid_for(VR::value_type value) {
      return false;
    }
  }

  template <typename T>
  void VR::validate_value_manipulator() const {
    if (!vr::manipulator_is_valid_for<T>(m_data.value)) {
      throw InvalidValueManipulator<T>(*this);
    }
  }
}

#include "vega/vr_constants.h"
