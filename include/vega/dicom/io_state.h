#pragma once

#include "vega/vega.h"

namespace vega {
  namespace dicom {
    class IOState {
      private:
        const Endian m_machine_endian;
        Endian m_dicom_endian;
        bool m_swap;

        bool m_vr_explicit;

      public:
        IOState();

        const Endian& dicom_endian() const;
        void set_dicom_endianness(const Endian& dicom_endian);

        bool vr_explicit() const;
        void set_vr_explicit(bool vr_explicit);

        bool swap() const;
    };
  }
}
