#include "vega/manipulators/unique_identifier_manipulator.h"

#include "vega/dicom/raw_reader.h"
#include "vega/dicom/raw_writer.h"
#include "vega/json.h"

#include <sstream>

namespace vega {
  namespace manipulators {
    UniqueIdentifierManipulator::UniqueIdentifierManipulator()
      : m_uid()
    {}

    UniqueIdentifierManipulator::UniqueIdentifierManipulator(std::shared_ptr<dicom::RawValue> raw_value)
    {
      std::string s = raw_value->str();

      if ((s.size() & 1) == 0 && s.back() == '\0') {
        m_uid = UID(s.substr(0, s.size()-1));
      }
      else {
        m_uid = UID(s);
      }
    }

    UniqueIdentifierManipulator::UniqueIdentifierManipulator(const std::string& str) {
      if ((str.size() & 1) == 0 && str.back() == '\0') {
        m_uid = UID(str.substr(0, str.size()-1));
      }
      else {
        m_uid = UID(str);
      }
    }

    UniqueIdentifierManipulator::UniqueIdentifierManipulator(const UID& uid)
      : m_uid(uid)
    {}

    UID& UniqueIdentifierManipulator::uid() { return m_uid; }
    const UID& UniqueIdentifierManipulator::uid() const { return m_uid; }

    std::shared_ptr<dicom::RawValue> UniqueIdentifierManipulator::raw_value() {
      std::shared_ptr<dicom::RawValue> raw_value = std::make_shared<dicom::RawValue>(
        m_uid.str().size() & 1 ? m_uid.str().size()+1 : m_uid.str().size()
      );

      size_t i;
      for (i = 0; i < m_uid.str().size(); ++i) {
        raw_value->data()[i] = m_uid.str()[i];
      }

      if (i & 1) raw_value->data()[i] = '\0';

      return raw_value;
    }

    std::string UniqueIdentifierManipulator::str() const { return m_uid.str(); }

    bool UniqueIdentifierManipulator::read_from(dicom::RawReader* reader, size_t num_bytes) {
      std::stringstream ss;
      char c;

      for (size_t i = 0; i < num_bytes; ++i) {
        if (!reader->read_into(&c)) return false;

        // Ignore last character if null
        if (c == '\0') {
          if (i != num_bytes-1) return false;
        }
        else {
          ss << c;
        }
      }

      m_uid = UID{ss.str()};
      return true;
    }

    size_t UniqueIdentifierManipulator::write_to(dicom::RawWriter* writer) const {
      writer->write_from(m_uid.str().begin(), m_uid.str().end());
      if (m_uid.str().size() & 1) {
        char c = '\0';
        writer->write_from(c);
        return m_uid.str().size()+1;
      }
      else {
        return m_uid.str().size();
      }
    }

    UID UniqueIdentifierManipulator::uid_from_raw_value(const std::shared_ptr<dicom::RawValue>& raw_value) {
      std::stringstream ss;

      for (size_t i = 0; i < raw_value->size(); ++i) {
        const char& c = raw_value->data()[i];
        if (c == '\0') break;

        ss << c;
      }

      return UID(ss.str());
    }

    void UniqueIdentifierManipulator::json(Formatter& formatter) const {
      formatter << vega::to_json(this->str());
    }

    void UniqueIdentifierManipulator::from_json(std::stringstream& json_string) {
      char c;
      json_string >> c;
      assert(c == '"');
      json_string >> m_uid;
      json_string >> c;
      assert(c == '"');
    }
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::UniqueIdentifierManipulator>(VR::value_type value) {
      return value == vr::UI_VALUE;
    }
  }
}
