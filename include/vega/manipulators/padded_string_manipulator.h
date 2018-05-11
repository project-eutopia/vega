#pragma once

#include "vega/uid.h"
#include "vega/manipulators/value_manipulator.h"
#include "vega/string.h"

#include <vector>
#include <string>

// AE: char, space padded, backslash delimited
//     Application Entity
// AS: char (4 bytes), space padding, backslash delimiter
//     Age String
// CS: char (leading/trailing spaces non-significant), space padding, backslash delimiter
//     Code string
// DA: char (8 bytes), space padding, backslash delimiter
//     Date (YYYYMMDD)
// DS: char (max 16 bytes), space padding, backslash delimiter
//     Decimal string
// DT: char (max 26 bytes), space padding, backslash delimiter
//     Datetime (YYYYMMDDHHMMSS.FFFFFF&ZZXX)
// IS: char (max 12), space padding, backslash delimiter
//     Integer string
// LO: char (max 64), space padding, backslash delimiter
//     Long string
// LT: char (max 10240, allow backslash), space padding, no delimiter (VM == 1)
//     Long text
// PN: char (max 64), space padding, backslash delimiter
//     Person name
// SH: char (max 16 leading/trailing space insignificant), space padding, backslash delimiter
//     Short string
// ST: char (max 1024, allow backslash, trailing space insignificant), space padding, no delimiter (VM == 1)
//     Short text
// TM: char (max 16), space padding, backslash delimiter
//     Time (HHMMSS.FFFFFF)
// UI: char (max 64), null padding, backslash delimiter
//     Attribute Tag

// UT: char (unlimited), space padding, no delimiter (VM == 1)
//     Attribute Tag

// AT: two uint16_t, no padding, no delimiter
//     Attribute Tag
// FL: float32, no padding, no delimiter
//     Floating point
// FD: double64, no padding, no delimiter
//     Floating point double
// SL: int32_t, no padding, no delimiter
//     Signed long
// SS: int16_t, no padding, no delimiter
//     Signed short
// UL: uint32_t, no padding, no delimiter
//     Unsigned long
// US: uint16_t, no padding, no delimiter
//     Unsigned short

// OB: bytes, null padding, no delimiter necessary
//     Other byte string
// OD: double64, no padding, no delimiter
//     Other double string
// OF: float32, no padding, no delimiter
//     Other float string
// OW: 16bit words, no padding, no delimiter
//     Other word string

// SQ: DataSets, no padding, no delimiter
//     Sequence

// UN: bytes, no padding, no delimiter
//     Unknown

// ox: OB or OF
// xs: SS or US
// xw: SS or US or OW
// uw: US or OW

namespace vega {
  namespace manipulators {
    /**
     * \brief A manipulator class for VR that represent elements which are encoded
     * in DICOM using strings.
     *
     * This manipulator inherits from `std::vector<T>` and therefore includes many
     * helpful methods for handling vectors of the input type T.  By only having to
     * deal with a vector of generic elements T, there is no need to concern oneself
     * with the conversion to the DICOM string.
     */
    template <typename T>
    class PaddedStringManipulator : public ValueManipulator, private std::vector<T> {
      private:
        typedef std::vector<T> base_vector;

      public:
        typedef typename base_vector::value_type      value_type;
        typedef typename base_vector::size_type       size_type;
        typedef typename base_vector::iterator        iterator;
        typedef typename base_vector::const_iterator  const_iterator;
        typedef typename base_vector::reference       reference;
        typedef typename base_vector::const_reference const_reference;

        using base_vector::operator[];
        using base_vector::at;

        using base_vector::clear;
        using base_vector::erase;
        using base_vector::reserve;
        using base_vector::resize;
        using base_vector::size;
        using base_vector::begin;
        using base_vector::end;
        using base_vector::front;
        using base_vector::back;
        using base_vector::push_back;
        using base_vector::emplace_back;

        PaddedStringManipulator();
        explicit PaddedStringManipulator(std::shared_ptr<dicom::RawValue> raw_value);
        explicit PaddedStringManipulator(const std::string& s);

        std::shared_ptr<dicom::RawValue> raw_value() override;
        std::string str() const override;
        bool read_from(dicom::RawReader* reader, size_t num_bytes) override;
        size_t write_to(dicom::RawWriter* writer) const override;

        virtual bool operator==(const ValueManipulator& other) const override;
        virtual bool operator!=(const ValueManipulator& other) const override;

        virtual void json(Formatter& formatter) const override;
        virtual void from_json(std::stringstream& json_string) override;

      private:
        // For parsing all elements from the raw DICOM string (including delimiters)
        void parse_from_string(const std::string& s);
    };
  }
}

#include "vega/manipulators/padded_string_manipulator_impl.h"
