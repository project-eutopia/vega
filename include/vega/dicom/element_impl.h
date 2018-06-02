namespace vega {
  namespace dicom {
    template <typename T>
    Element<T>::Element()
    {
      static_assert(vega::dictionary::HasTag<T>::value, "type must have Tag");
      m_data_element = std::make_shared<DataElement>(T::tag, T::vr);
    }

    template <typename T>
    Element<T>::Element(const Tag& tag)
      : m_data_element(std::make_shared<DataElement>(tag, T::vr))
    {
      if (!T::tag_mask.contains(tag)) {
        throw vega::Exception("Element<T>(const Tag&): Tag must agreen with type T.");
      }
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
    const Tag& Element<T>::tag() const { return m_data_element->tag(); }

    template <typename T>
    const VR& Element<T>::vr() const { return m_data_element->vr(); }

    template <typename T>
    const DataElementHeader::length_type& Element<T>::length() const { return m_data_element->length(); }

    template <typename T>
    DataElementHeader::length_type& Element<T>::length() { return m_data_element->length(); }
  }
}
