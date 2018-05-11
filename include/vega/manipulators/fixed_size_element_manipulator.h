#pragma once

#include "vega/uid.h"
#include "vega/manipulators/value_manipulator.h"
#include "vega/string.h"

#include <vector>
#include <string>

namespace vega {
  namespace manipulators {
    /**
     * \brief A manipulator class for VR that represent fixed size elements like
     * integers and floating point numbers.
     *
     * This manipulator inherits from `std::vector<T>` and therefore includes many
     * helpful methods for handling vectors of the input type T.
     */
    template <typename T>
    class FixedSizeElementManipulator : public ValueManipulator, private std::vector<T> {
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

        FixedSizeElementManipulator(const_iterator first, const_iterator last);

        FixedSizeElementManipulator();
        explicit FixedSizeElementManipulator(std::shared_ptr<dicom::RawValue> raw_value);

        template <typename U>
        U& get(size_t i) {
          static_assert(sizeof(U) == sizeof(T), "Type T and U must have same size in FixedSizeElementManipulator::get()");
          return *((U*)&(*this)[i]);
        }

        template <typename U>
        const U& get(size_t i) const {
          static_assert(sizeof(U) == sizeof(T), "Type T and U must have same size in FixedSizeElementManipulator::get()");
          return *((const U*)&(*this)[i]);
        }

        virtual std::shared_ptr<dicom::RawValue> raw_value() override;
        virtual std::string str() const override;
        virtual bool read_from(dicom::RawReader* reader, size_t num_bytes) override;
        virtual size_t write_to(dicom::RawWriter* writer) const override;

        virtual bool operator==(const ValueManipulator& other) const override;
        virtual bool operator!=(const ValueManipulator& other) const override;

        virtual void json(Formatter& formatter) const override;
        virtual void from_json(std::stringstream& json_string) override;
    };
  }
}

#include "vega/manipulators/fixed_size_element_manipulator_impl.h"
