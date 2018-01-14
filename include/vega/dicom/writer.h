#pragma once

#include <ostream>
#include <string>
#include <vector>
#include <memory>

#include "vega/vega.h"
#include "vega/tag.h"
#include "vega/vr.h"
#include "vega/dicom/io_state.h"
#include "vega/dicom/raw_writer.h"
#include "vega/dicom/data_element.h"
#include "vega/dicom/data_set.h"
#include "vega/dictionary/dictionary.h"

namespace vega {
  namespace dicom {
    class Writer {
      public:
        class WritingError : public vega::Exception { using vega::Exception::Exception; };

      private:
        RawWriter m_raw_writer;

      public:
        explicit Writer(std::shared_ptr<std::ostream> os);

        RawWriter& raw_writer();

        const Endian& dicom_endian() const;
        void set_dicom_endianness(const Endian& dicom_endian);

        bool vr_explicit() const;
        void set_vr_explicit(bool vr_explicit);

        std::streampos tell();
        void seek_pos(std::streampos pos);
        void seek_delta(std::streampos delta);

        size_t write_element(std::shared_ptr<DataElement> element);
        size_t write_data_set(std::shared_ptr<DataSet> data_set);
    };
  }
}

