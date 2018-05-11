#pragma once

#include "vega/types.h"
#include "vega/manipulators/value_manipulator.h"

namespace vega {
  namespace manipulators {
    // TODO:  Properly handle different bit encodings
    class EncapsulatedPixelDataManipulator : public ValueManipulator {
      private:
        std::vector<uint32_t> m_offsets;
        std::vector<std::vector<Byte>> m_fragments;

      public:
        virtual std::shared_ptr<dicom::RawValue> raw_value() override;
        virtual std::string str() const override;
        virtual bool read_from(dicom::RawReader* reader, size_t num_bytes) override;
        virtual size_t write_to(dicom::RawWriter* writer) const override;

        virtual bool operator==(const ValueManipulator& other) const override;
        virtual bool operator!=(const ValueManipulator& other) const override;

        static bool allows_vr(const VR& vr) {
          return vr == vr::OB || vr == vr::OW || vr == vr::OX;
        }

        virtual bool is_valid_for(const VR& vr) const override {
          return EncapsulatedPixelDataManipulator::allows_vr(vr);
        }

        virtual void json(Formatter& formatter) const override;
        virtual void from_json(std::stringstream& json_string) override;
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::EncapsulatedPixelDataManipulator>(VR::value_type value);
  }
}
