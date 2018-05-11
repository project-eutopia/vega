#pragma once

#include "vega/manipulators/value_manipulator.h"
#include <string>

namespace vega {
  namespace manipulators {
    /**
     * \brief A manipulator class for VR that represent a single string.
     *
     * The string stored by this manipulator can be accessed through the value() method.
     */
    class SingleStringManipulator : public ValueManipulator {
      private:
        std::string m_str;

      public:
        SingleStringManipulator();
        explicit SingleStringManipulator(const std::string& str);
        explicit SingleStringManipulator(std::shared_ptr<dicom::RawValue> raw_value);

        /// \return a reference to the internal string wrapped by this manipulator.
        const std::string& value() const { return m_str; }
        /// \return a reference to the internal string wrapped by this manipulator.
        std::string& value() { return m_str; }

        std::shared_ptr<dicom::RawValue> raw_value() override;
        std::string str() const override;
        bool read_from(dicom::RawReader* reader, size_t num_bytes) override;
        size_t write_to(dicom::RawWriter* writer) const override;

        virtual bool operator==(const ValueManipulator& other) const override;
        virtual bool operator!=(const ValueManipulator& other) const override;

        virtual void json(Formatter& formatter) const override;
        virtual void from_json(std::stringstream& json_string) override;
    };
  }
}
