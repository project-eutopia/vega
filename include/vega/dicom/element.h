#pragma once

#include <memory>
#include "vega/dicom/data_element.h"

namespace vega {
  namespace dicom {
    template <typename T>
    class Element {
      private:
        std::shared_ptr<DataElement> m_data_element;

      public:
        Element();
        explicit Element(const Tag& tag);
        explicit Element(const std::shared_ptr<DataElement>& data_element);

        std::shared_ptr<DataElement> underlying_data_element() const;
        std::shared_ptr<typename T::manipulator_type> manipulator();

        const Tag& tag() const;
        Tag& tag();

        const VR& vr() const;
        VR& vr();
    };
  }
}

#include "vega/dicom/element_impl.h"
