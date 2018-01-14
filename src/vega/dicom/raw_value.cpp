#include "vega/dicom/raw_value.h"

namespace vega {
  namespace dicom {
    RawValue::RawValue() : RawValue(0)
    {}

    RawValue::RawValue(size_t num_bytes)
      : m_bytes(std::make_shared<RawValue::byte_buffer_type>(num_bytes))
    {}

    RawValue::RawValue(const std::string& s)
      : RawValue(s.c_str(), s.c_str() + s.size())
    {}

    RawValue::RawValue(const char * const begin, const char * const end)
      : m_bytes(std::make_shared<byte_buffer_type>(begin, end))
    {}

    std::shared_ptr<RawValue::byte_buffer_type> RawValue::bytes() {
      return m_bytes;
    }

    void RawValue::set_bytes(std::shared_ptr<RawValue::byte_buffer_type> bytes) {
      m_bytes = bytes;
    }

    void RawValue::resize(size_t num_bytes) {
      m_bytes->resize(num_bytes);
    }

    size_t RawValue::size() const { return m_bytes->size(); }

    char* RawValue::data() { return m_bytes->data(); }
    const char* RawValue::data() const { return m_bytes->data(); }

    std::string RawValue::str() const {
      return std::string(this->begin(), this->end());
    }

    RawValue::byte_buffer_type::iterator RawValue::begin() { return m_bytes->begin(); }
    RawValue::byte_buffer_type::const_iterator RawValue::begin() const { return m_bytes->begin(); };

    RawValue::byte_buffer_type::iterator RawValue::end() { return m_bytes->end(); }
    RawValue::byte_buffer_type::const_iterator RawValue::end() const { return m_bytes->end(); };
  }
}
