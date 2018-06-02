#include "vega/dicom/writer.h"

namespace vega {
  namespace dicom {
    Writer::Writer(std::shared_ptr<std::ostream> os)
      : m_raw_writer(os)
    {}

    RawWriter& Writer::raw_writer() { return m_raw_writer; }

    const Endian& Writer::dicom_endian() const { return m_raw_writer.dicom_endian(); }
    void Writer::set_dicom_endianness(const Endian& dicom_endian) { m_raw_writer.set_dicom_endianness(dicom_endian); }

    bool Writer::vr_explicit() const { return m_raw_writer.vr_explicit(); }
    void Writer::set_vr_explicit(bool vr_explicit) { m_raw_writer.set_vr_explicit(vr_explicit); }

    std::streampos Writer::tell() {
      return m_raw_writer.tell();
    }

    void Writer::seek_pos(std::streampos pos) {
      m_raw_writer.seek_pos(pos);
    }

    void Writer::seek_delta(std::streampos delta) {
      m_raw_writer.seek_delta(delta);
    }

    // TODO add const everywhere
    size_t Writer::write_element(std::shared_ptr<DataElement> element) {
      size_t prefix_bytes = 0;
      /* std::cout << "Writer::write_element, tag = " << element->tag() << " at " << std::endl; */
      prefix_bytes += m_raw_writer.write_from(element->tag());

      std::streampos length_pos;
      uint32_t length_bytes;

      if (this->vr_explicit()) {
        // Read explicit VR
        /* std::cout << "Writer::write_element, explicit VR = " << element->vr() << std::endl; */
        prefix_bytes += m_raw_writer.write_from(element->vr());

        // Here, we have a 2 byte buffer, and then 32 bit length field
        if (element->vr().needs_two_byte_padding()) {
          uint16_t zero = 0;
          /* std::cout << "Writer::write_element, 2 byte buffer" << std::endl; */
          prefix_bytes += m_raw_writer.write_from(zero);
          length_bytes = sizeof(element->length());
          /* std::cout << "Writer::write_element, 4 byte length temp value " << element->length() << std::endl; */
          length_pos = this->tell();
          prefix_bytes += m_raw_writer.write_from(element->length());
        }
        // Here we have 16 bit length field
        else {
          uint16_t length = element->length();
          length_bytes = sizeof(length);
          /* std::cout << "Writer::write_element, 2 byte length temp value " << length << std::endl; */
          length_pos = this->tell();
          prefix_bytes += m_raw_writer.write_from(length);
        }
      }
      else {
        // Implicit VR
        length_pos = this->tell();
        length_bytes = sizeof(element->length());
        /* std::cout << "Writer::write_element, implicit VR, 4 byte length temp value " << element->length() << std::endl; */
        prefix_bytes += m_raw_writer.write_from(element->length());
      }

      uint32_t value_length = 0;
      if (element->is_sequence()) {
        for (auto data_set : *element) {
          /* std::cout << "Writer::write_element, writing data set" << std::endl; */
          value_length += this->write_data_set(data_set);
        }
        if (element->is_undefined_length()) {
          // Require sequence delimination item
          /* std::cout << "Writer::write_element, writing sequence delimination item" << std::endl; */
          value_length += m_raw_writer.write_from(Tag::SEQ_DELIMITATION);
          uint32_t zero = 0;
          value_length += m_raw_writer.write_from(zero);
        }
      }
      else {
        /* std::cout << "Writer::write_element, writing manipulator" << std::endl; */
        value_length += element->manipulator()->write_to(&m_raw_writer);
      }

      if (!element->is_undefined_length()) {
        // Here we go back an retroactively record how many bytes we wrote
        std::streampos end_of_element = this->tell();
        this->seek_pos(length_pos);
        if (length_bytes == sizeof(value_length)) {
          /* std::cout << "Writer::write_element, writing correct 4 byte length " << value_length << std::endl; */
          m_raw_writer.write_from(value_length);
        }
        else {
          uint16_t l = value_length;
          if (l < value_length) {
            throw std::runtime_error("Error: encountered overflow when trying to write 2 byte length");
          }
          /* std::cout << "Writer::write_element, writing correct 2 byte length " << l << std::endl; */
          m_raw_writer.write_from(l);
        }

        this->seek_pos(end_of_element);
      }

      return prefix_bytes + value_length;
    }

    size_t Writer::write_data_set(std::shared_ptr<DataSet> data_set) {
      size_t prefix_bytes = 0;
      uint32_t length = 0;

      std::streampos length_pos;

      prefix_bytes += m_raw_writer.write_from(Tag::ITEM);
      length_pos = this->tell();
      prefix_bytes += m_raw_writer.write_from(length);

      for (auto element : *data_set) {
        length += this->write_element(element);
      }

      std::streampos end_of_data_set = this->tell();
      this->seek_pos(length_pos);
      m_raw_writer.write_from(length);
      this->seek_pos(end_of_data_set);

      return prefix_bytes + length;
    }
  }
}
