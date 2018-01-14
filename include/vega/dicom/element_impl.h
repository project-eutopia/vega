#include "vega/dictionary_data.h"

namespace vega {
  namespace dicom {
    template <typename T>
    Element<T>::Element()
      : m_data_element(std::make_shared<DataElement>(T::tag, T::vr))
    {
      static_assert(vega::dictionary::HasTag<T>::value, "type must have Tag");
    }

    template <typename T>
    Element<T>::Element(const Tag& tag)
      : m_data_element(std::make_shared<DataElement>(tag, T::vr))
    {
    }

    template <typename T>
    Element<T>::Element(const std::shared_ptr<DataElement>& data_element)
      : m_data_element(data_element)
    {
      // Must have correct contents
      if (!T::tag_mask.contains(m_data_element->tag())) throw vega::Exception("must have Tag that agrees with type");
      if (T::vr != m_data_element->vr()) throw vega::Exception("must have VR that agrees with type");
    }

    template <typename T>
    std::shared_ptr<DataElement> Element<T>::underlying_data_element() const {
      return m_data_element;
    }

    template <typename T>
    std::shared_ptr<typename T::manipulator_type> Element<T>::manipulator() {
      return m_data_element->get_manipulator<typename T::manipulator_type>();
    }

    template <typename T>
    const Tag& Element<T>::tag() const { return m_data_element->tag(); }

    template <typename T>
    Tag& Element<T>::tag() { return m_data_element->tag(); }

    template <typename T>
    const VR& Element<T>::vr() const { return m_data_element->vr(); }
    template <typename T>
    VR& Element<T>::vr() { return m_data_element->vr(); }
  }
}
