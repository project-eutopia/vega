#include "vega/manipulators/single_string_manipulator.h"
#include "vega/json.h"

#include "vega/dicom/raw_reader.h"
#include "vega/dicom/raw_writer.h"

#include <sstream>

namespace vega {
  namespace manipulators {
    SingleStringManipulator::SingleStringManipulator()
      : m_str()
    {
    }
    SingleStringManipulator::SingleStringManipulator(const std::string& str)
      : m_str(str)
    {
    }
    SingleStringManipulator::SingleStringManipulator(std::shared_ptr<dicom::RawValue> raw_value)
      : m_str(raw_value->str())
    {
    }

    std::shared_ptr<dicom::RawValue> SingleStringManipulator::raw_value() {
      return std::make_shared<dicom::RawValue>(m_str);
    }

    std::string SingleStringManipulator::str() const {
      if (m_str.size() & 1) {
        // Odd, so padd
        std::stringstream ss;
        ss << m_str << ' ';
        return ss.str();
      }
      else {
        // Even, so no need for padding
        return m_str;
      }
    }

    bool SingleStringManipulator::read_from(dicom::RawReader* reader, size_t num_bytes) {
      if (num_bytes == 0) {
        m_str = "";
        return true;
      }

      std::stringstream ss;
      char c;

      for (size_t i = 0; i < num_bytes; ++i) {
        if (!reader->read_into(&c)) return false;
        ss << c;
      }

      m_str = ss.str();
      return true;
    }

    size_t SingleStringManipulator::write_to(dicom::RawWriter* writer) const {
      std::string s = this->str();
      return writer->write_from(s.begin(), s.end());
    }

    bool SingleStringManipulator::operator==(const ValueManipulator& other) const {
      const SingleStringManipulator* other_ptr = dynamic_cast<const SingleStringManipulator*>(&other);
      if (!other_ptr) return false;

      return this->value() == other_ptr->value();
    }

    bool SingleStringManipulator::operator!=(const ValueManipulator& other) const {
      return !(*this == other);
    }

    void SingleStringManipulator::json(Formatter& formatter) const {
      Json::to_json(formatter, *this);
    }

    void SingleStringManipulator::from_json(std::stringstream& json_string) {
      m_str = Json::value_from_json<std::string>(json_string);
    }
  }
}
