#include "vega/dicom/raw_reader.h"

namespace vega {
  namespace dicom {
    RawReader::RawReader(std::shared_ptr<std::istream> is)
      :
        m_state(),
        m_is(is)
    {
      // Remember current position
      auto cur = m_is->tellg();
      m_is->seekg(0, m_is->end);
      m_eof_pos = m_is->tellg();
      m_is->seekg(cur, m_is->beg);
    }

    const Endian& RawReader::dicom_endian() const { return m_state.dicom_endian(); }
    void RawReader::set_dicom_endianness(const Endian& dicom_endian) { m_state.set_dicom_endianness(dicom_endian); }

    bool RawReader::vr_explicit() const { return m_state.vr_explicit(); }
    void RawReader::set_vr_explicit(bool vr_explicit) { m_state.set_vr_explicit(vr_explicit); }

    bool RawReader::swap() const { return m_state.swap(); }

    std::streampos RawReader::tell() {
      return m_is->tellg();
    }

    void RawReader::seek_pos(std::streampos pos) {
      m_is->seekg(pos, m_is->beg);
    }

    void RawReader::seek_delta(std::streampos delta) {
      m_is->seekg(delta, m_is->cur);
    }

    std::streampos RawReader::eof_pos() const {
      return m_eof_pos;
    }

    bool RawReader::eof() {
      return this->tell() < 0 || this->tell() >= this->eof_pos();
    }

    void RawReader::rewind() {
      m_is->seekg(0, m_is->beg);
    }

    template <>
    bool RawReader::read_into(Tag* tag) {
      if (!this->read_into(&tag->group())) return false;
      if (!this->read_into(&tag->element())) return false;
      return true;
    }

    template<>
    bool RawReader::read_into(VR* vr) {
      if (!this->read_into(&vr->data().characters[0])) return false;
      if (!this->read_into(&vr->data().characters[1])) return false;
      return true;
    }
  }
}
