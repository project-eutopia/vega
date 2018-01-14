#pragma once

#include <vector>
#include <memory>

#include "vega/vega.h"
#include "vega/vr.h"

namespace vega {
  namespace dicom {
    class RawValue {
      public:
        class InvalidRawValue : public vega::Exception {};

        typedef std::vector<char> byte_buffer_type;

      private:
        std::shared_ptr<byte_buffer_type> m_bytes;

      public:
        RawValue();
        explicit RawValue(size_t num_bytes);
        explicit RawValue(const std::string& s);
        RawValue(const char * const begin, const char * const end);

        std::shared_ptr<byte_buffer_type> bytes();

        void set_bytes(std::shared_ptr<byte_buffer_type> bytes);

        void resize(size_t num_bytes);
        size_t size() const;

        char* data();
        const char* data() const;

        std::string str() const;

        byte_buffer_type::iterator begin();
        byte_buffer_type::const_iterator begin() const;

        byte_buffer_type::iterator end();
        byte_buffer_type::const_iterator end() const;
    };
  }
}
