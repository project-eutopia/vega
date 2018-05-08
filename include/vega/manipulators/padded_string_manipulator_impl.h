#include "vega/dicom/raw_reader.h"
#include "vega/dicom/raw_writer.h"
#include "vega/json.h"

#include <sstream>

namespace vega {
  namespace manipulators {
    template <typename T>
    PaddedStringManipulator<T>::PaddedStringManipulator()
      : PaddedStringManipulator::base_vector()
    {}

    template <typename T>
    PaddedStringManipulator<T>::PaddedStringManipulator(std::shared_ptr<dicom::RawValue> raw_value)
      : PaddedStringManipulator(raw_value->str())
    {}

    template <typename T>
    PaddedStringManipulator<T>::PaddedStringManipulator(const std::string& s)
      : PaddedStringManipulator::base_vector()
    {
      this->parse_from_string(s);
    }

    template <typename T>
    std::shared_ptr<dicom::RawValue> PaddedStringManipulator<T>::raw_value() {
      return std::make_shared<dicom::RawValue>(this->str());
    }

    template <typename T>
    std::string PaddedStringManipulator<T>::str() const {
      std::stringstream ss;
      size_t bytes = 0;

      for (size_t i = 0; i < this->size(); ++i) {
        auto s = vega::to_string<T>((*this)[i]);
        ss << s;

        bytes += s.size();
        if (i < this->size()-1) {
          ss << '\\';
          bytes += 1;
        }
      }

      if (bytes & 1) {
        ss << ' ';
        bytes += 1;
      }

      return ss.str();
    }

    template <typename T>
    bool PaddedStringManipulator<T>::read_from(dicom::RawReader* reader, size_t num_bytes) {
      if (num_bytes == 0) {
        this->clear();
        return true;
      }

      std::stringstream ss;
      char c;

      size_t delimiters = 0;

      for (size_t i = 0; i < num_bytes; ++i) {
        if (!reader->read_into(&c)) return false;
        if (c == '\\') ++delimiters;
        ss << c;
      }

      this->clear();
      this->reserve(delimiters+1);

      this->parse_from_string(ss.str());
      return true;
    }

    template <typename T>
    size_t PaddedStringManipulator<T>::write_to(dicom::RawWriter* writer) const {
      std::string s = this->str();
      return writer->write_from(s.begin(), s.end());
    }

    template <typename T>
    bool PaddedStringManipulator<T>::operator==(const ValueManipulator& other) const {
      const PaddedStringManipulator<T>* other_ptr = dynamic_cast<const PaddedStringManipulator<T>*>(&other);
      if (!other_ptr) return false;

      return this->str() == other_ptr->str();
    }

    template <typename T>
    bool PaddedStringManipulator<T>::operator!=(const ValueManipulator& other) const {
      return !(*this == other);
    }

    template <typename T>
    void PaddedStringManipulator<T>::parse_from_string(const std::string& s) {
      std::istringstream ss(s);
      std::string element_string;

      this->clear();
      while(std::getline(ss, element_string, '\\')) {
        // Trim padding if present
        if (element_string.back() == ' ') {
          element_string = element_string.substr(0, element_string.size()-1);
        }
        this->push_back(vega::from_string<T>(element_string));
      }
    }

    template <typename T>
    void PaddedStringManipulator<T>::json(Formatter& formatter) const {
      Json::to_json(formatter, *this);
    }

    template <typename T>
    void PaddedStringManipulator<T>::from_json(std::stringstream& json_string) {
      Json::from_json<decltype(*this), T>(json_string, *this);
    }
  }
}
