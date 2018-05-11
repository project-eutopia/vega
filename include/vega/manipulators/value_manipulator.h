#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <memory>

#include "vega/vega.h"
#include "vega/formatter.h"
#include "vega/dicom/raw_value.h"

// Base abstract manipulator class
// Used for reading/writing
// Can be overridden to have more complicated manipulators,
// such as PixelDataManipulator, DecimalStringManipulator, or TypedManipulator<T>
// which offer useful ways of manipulating DICOM data (e.g. pixel_data(yi,xi) = value;)
namespace vega {
  namespace dicom {
    class RawReader;
    class RawWriter;
  }

  namespace manipulators {

    // NOTE:  Implementations of this class should also implement
    //
    // template <>
    // bool vr::manipulator_is_valid_for<T>(VR::value_type value);
    //
    // To return true if it is a valid VR
    class ValueManipulator {
      public:
        virtual ~ValueManipulator() = 0;

        // Returns a RawValue that is suitable for writing/reading to/from DICOM
        // Acts as a common link between different manipulators
        virtual std::shared_ptr<dicom::RawValue> raw_value() = 0;
        virtual std::string str() const = 0;
        virtual bool read_from(dicom::RawReader* reader, size_t num_bytes) = 0;
        virtual size_t write_to(dicom::RawWriter* writer) const = 0;
        // Override for allowed VR
        virtual bool is_valid_for(const VR& vr) const { return false; }

        virtual bool operator==(const ValueManipulator& other) const;
        virtual bool operator!=(const ValueManipulator& other) const;

        virtual void json(Formatter& formatter) const = 0;
        virtual void from_json(std::stringstream& json_string) = 0;
    };
  }
}
