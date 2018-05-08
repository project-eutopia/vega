#pragma once

#include "vega/uid.h"
#include "vega/manipulators/value_manipulator.h"

namespace vega {
  namespace manipulators {
    class UniqueIdentifierManipulator : public ValueManipulator {
      private:
        UID m_uid;

      public:
        UniqueIdentifierManipulator();
        explicit UniqueIdentifierManipulator(std::shared_ptr<dicom::RawValue> raw_value);
        explicit UniqueIdentifierManipulator(const std::string& str);
        explicit UniqueIdentifierManipulator(const UID& uid);

        UID& uid();
        const UID& uid() const;

        std::shared_ptr<dicom::RawValue> raw_value() override;
        std::string str() const override;
        bool read_from(dicom::RawReader* reader, size_t num_bytes) override;
        size_t write_to(dicom::RawWriter* writer) const override;

        virtual bool is_valid_for(const VR& vr) const override {
          return vr == vr::UI;
        }

        virtual void json(Formatter& formatter) const override;
        virtual void from_json(std::stringstream& json_string) override;

      private:
        static UID uid_from_raw_value(const std::shared_ptr<dicom::RawValue>& raw_value);
    };
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UniqueIdentifierManipulator>(VR::value_type value);
  }
}
