#include "vega/dicom/raw_reader.h"
#include "vega/dicom/raw_writer.h"
#include "vega/json.h"
#include "vega/vega.h"

#include <sstream>
#include <algorithm>

namespace vega {
  namespace manipulators {
    template <typename T>
    FixedSizeElementManipulator<T>::FixedSizeElementManipulator(const_iterator first, const_iterator last)
      : FixedSizeElementManipulator::base_vector(first, last)
    {}

    template <typename T>
    FixedSizeElementManipulator<T>::FixedSizeElementManipulator()
      : FixedSizeElementManipulator::base_vector()
    {}

    template <typename T>
    FixedSizeElementManipulator<T>::FixedSizeElementManipulator(std::shared_ptr<dicom::RawValue> raw_value)
    {
      if (raw_value->size() % sizeof(T) != 0) throw vega::Exception("Incompatible size for FixedSizeElementManipulator");

      std::copy(
        reinterpret_cast<const T * const>(&*raw_value->begin()),
        reinterpret_cast<const T * const>(&*raw_value->end()),
        std::back_inserter(*this)
      );
    }

    template <typename T>
    std::shared_ptr<dicom::RawValue> FixedSizeElementManipulator<T>::raw_value() {
      return std::make_shared<dicom::RawValue>(
        reinterpret_cast<const char * const>(&*this->begin()),
        reinterpret_cast<const char * const>(&*this->end())
      );
    }

    template <typename T>
    std::string FixedSizeElementManipulator<T>::str() const {
      std::stringstream ss;

      for (size_t i = 0; i < this->size(); ++i) {
        ss << vega::to_string<T>((*this)[i]);
        if (i < this->size()-1) {
          ss << '\\';
        }
      }

      return ss.str();
    }

    template <typename T>
    bool FixedSizeElementManipulator<T>::read_from(dicom::RawReader* reader, size_t num_bytes) {
      this->clear();

      if (num_bytes % sizeof(T) != 0) throw vega::Exception("Incompatible num_bytes for FixedSizeElementManipulator");

      for (size_t i = 0; i < num_bytes; i += sizeof(T)) {
        this->emplace_back();
        if (!reader->read_into(&this->back())) return false;
      }

      return true;
    }

    template <typename T>
    size_t FixedSizeElementManipulator<T>::write_to(dicom::RawWriter* writer) const {
      return writer->write_from(this->begin(), this->end());
    }

    template <typename T>
    bool FixedSizeElementManipulator<T>::operator==(const ValueManipulator& other) const {
      const FixedSizeElementManipulator<T>* other_ptr = dynamic_cast<const FixedSizeElementManipulator<T>*>(&other);
      if (!other_ptr) return false;

      if (this->size() != other_ptr->size()) return false;
      return std::equal(this->begin(), this->end(), other_ptr->begin());
    }

    template <typename T>
    bool FixedSizeElementManipulator<T>::operator!=(const ValueManipulator& other) const {
      return !(*this == other);
    }

    template <typename T>
    void FixedSizeElementManipulator<T>::json(Formatter& formatter) const {
      Json::to_json(formatter, *this);
    }

    template <typename T>
    void FixedSizeElementManipulator<T>::from_json(std::stringstream& json_string) {
      Json::from_json<decltype(*this), T>(json_string, *this);
    }
  }
}
