#pragma once

#include <istream>
#include <memory>

#include "vega/vega.h"
#include "vega/tag.h"
#include "vega/vr.h"
#include "vega/dicom/io_state.h"

namespace vega {
  namespace dicom {
    class RawReader {
      public:
        class ReadingError : public vega::Exception { using vega::Exception::Exception; };

      private:
        IOState m_state;

        std::shared_ptr<std::istream> m_is;
        std::streampos m_eof_pos;

      public:
        RawReader(std::shared_ptr<std::istream> is);

        const Endian& dicom_endian() const;
        void set_dicom_endianness(const Endian& dicom_endian);

        bool vr_explicit() const;
        void set_vr_explicit(bool vr_explicit);

        bool swap() const;

        template <typename Iter>
        bool read_into(Iter begin, Iter end);

        // Reads a single item
        template <typename T>
        bool read_into(T* ptr);

        bool eof();
        void rewind();
        std::streampos tell();
        std::streampos eof_pos() const;
        void seek_pos(std::streampos pos);
        void seek_delta(std::streampos delta);
    };

    template<>
    bool RawReader::read_into(Tag* tag);

    template<>
    bool RawReader::read_into(VR* vr);
  }
}

#include "vega/dicom/raw_reader_impl.h"
