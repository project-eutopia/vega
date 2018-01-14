#include "vega/dicom/io_state.h"

namespace vega {
  namespace dicom {
    IOState::IOState()
      :
        m_machine_endian(get_machine_endian()),
        m_dicom_endian(get_machine_endian()),
        m_swap(false),
        m_vr_explicit(true)
    {}

    const Endian& IOState::dicom_endian() const { return m_dicom_endian; }

    void IOState::set_dicom_endianness(const Endian& dicom_endian) {
      m_dicom_endian = dicom_endian;
      m_swap = (m_machine_endian != m_dicom_endian);
    }

    bool IOState::vr_explicit() const { return m_vr_explicit; }
    void IOState::set_vr_explicit(bool vr_explicit) { m_vr_explicit = vr_explicit; }

    bool IOState::swap() const { return m_swap; }
  }
}
