#include "vega/dicom/raw_writer.h"

namespace vega {
  namespace dicom {
    RawWriter::RawWriter(std::shared_ptr<std::ostream> os)
      :
        m_state(),
        m_os(os)
    {
      if (!m_os || !*m_os) {
        throw WritingError("Invalid output stream in RawWriter()");
      }
      m_os->seekp(0, m_os->beg);
    }

    const Endian& RawWriter::dicom_endian() const { return m_state.dicom_endian(); }
    void RawWriter::set_dicom_endianness(const Endian& dicom_endian) { m_state.set_dicom_endianness(dicom_endian); }

    bool RawWriter::vr_explicit() const { return m_state.vr_explicit(); }
    void RawWriter::set_vr_explicit(bool vr_explicit) { m_state.set_vr_explicit(vr_explicit); }

    std::streampos RawWriter::tell() {
      return m_os->tellp();
    }

    void RawWriter::seek_pos(std::streampos pos) {
      m_os->seekp(pos, m_os->beg);
    }

    void RawWriter::seek_delta(std::streampos delta) {
      m_os->seekp(delta, m_os->cur);
    }

    template <>
    size_t RawWriter::write_from(const Tag& tag) {
      /* std::cout << "write_from<Tag>, " << tag.str() << std::endl; */
      return this->write_from(tag.group()) + this->write_from(tag.element());
    }

    template <>
    size_t RawWriter::write_from(const VR& vr) {
      /* std::cout << "write_from<VR>, " << vr.str() << std::endl; */
      return this->write_from(vr.data().characters[0]) + this->write_from(vr.data().characters[1]);
    }
  }
}
