#include "vega/dicom/preamble.h"

#include "vega/vega.h"
#include "vega/dicom/writer.h"

namespace vega {
  namespace dicom {
    Preamble::Preamble()
      : m_present(true)
    {}

    Preamble::Preamble(Reader& reader)
      : m_present(true)
    {
      // Preamble is written in little endian
      const Endian original_endian = reader.dicom_endian();
      reader.set_dicom_endianness(Endian::LITTLE);

      // Skip 128 bytes
      reader.seek_delta(128);
      // Read DICM characters
      std::vector<char> dicm_buffer(4);
      if (!reader.raw_reader().read_into<decltype(dicm_buffer)::iterator>(dicm_buffer.begin(), dicm_buffer.end())) {
        m_present = false;
        reader.rewind();
      }
      else {
        std::string dicm(dicm_buffer.data(), 4);

        if (dicm != std::string("DICM")) {
          m_present = false;
          reader.rewind();
        }
      }

      reader.set_dicom_endianness(original_endian);
    }

    bool Preamble::present() const { return m_present; }

    size_t Preamble::write(std::shared_ptr<Writer> writer) {
      if (!this->present()) return 0;

      unsigned int zeros = 0;
      for (size_t i = 0; i < 128 / sizeof(zeros); ++i) {
        writer->raw_writer().write_from<decltype(zeros)>(zeros);
      }
      std::vector<char> dicm = {'D', 'I', 'C', 'M'};
      return 128 + writer->raw_writer().write_from<decltype(dicm)::iterator>(dicm.begin(), dicm.end());
    }
  }
}
