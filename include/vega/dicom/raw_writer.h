#pragma once

#include <ostream>
#include <memory>

#include "vega/vega.h"
#include "vega/tag.h"
#include "vega/vr.h"
#include "vega/dicom/io_state.h"

namespace vega {
  namespace dicom {
    class RawWriter {
      public:
        class WritingError : public vega::Exception { using vega::Exception::Exception; };

      private:
        IOState m_state;
        std::shared_ptr<std::ostream> m_os;

      public:
        explicit RawWriter(std::shared_ptr<std::ostream> os);

        const Endian& dicom_endian() const;
        void set_dicom_endianness(const Endian& dicom_endian);

        bool vr_explicit() const;
        void set_vr_explicit(bool vr_explicit);

        std::streampos tell();
        void seek_pos(std::streampos pos);
        void seek_delta(std::streampos delta);

        template <typename Iter>
        size_t write_from(Iter begin, Iter end) {
          size_t bytes = 0;
          for(auto it = begin; it != end; ++it) {
            bytes += this->write_from(*it);
          }
          return bytes;
        }

        // Define by do not implement -- require usage of reference template below
        template <typename T>
        size_t write_from(const T* val);

        template <typename T>
        size_t write_from(const T& val) {
          if (m_state.swap()) {
            // Have to swap the bytes to fix endian mis-match
            const char* raw = reinterpret_cast<const char*>(&val);
            for (size_t j = 0; j < sizeof(T); ++j) {
              m_os->write(raw + sizeof(T)-j-1, 1);
            }
          }
          else {
            // Read in bytes directly
            const char* raw = reinterpret_cast<const char*>(&val);
            m_os->write(raw, sizeof(T));
          }

          return sizeof(T);
        }
    };

    template <>
    size_t RawWriter::write_from(const Tag& tag);

    template<>
    size_t RawWriter::write_from(const VR& vr);
  }
}

