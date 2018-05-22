#include "vega/dicom/reader.h"
#include "vega/dictionary/dictionary.h"

#include "vega/manipulator.h"

#include <iostream>

namespace vega {
  namespace dicom {
    Reader::Reader(std::shared_ptr<std::istream> is, bool allow_any_explicit_vr, bool lazy_load)
      :
        m_formatter(std::cout, false),
        m_raw_reader(is),
        m_allow_any_explicit_vr(allow_any_explicit_vr),
        m_lazy_load(lazy_load)
    {
    }

    RawReader& Reader::raw_reader() {
      return m_raw_reader;
    }

    const Endian& Reader::dicom_endian() const { return m_raw_reader.dicom_endian(); }
    void Reader::set_dicom_endianness(const Endian& dicom_endian) { m_raw_reader.set_dicom_endianness(dicom_endian); }

    bool Reader::vr_explicit() const { return m_raw_reader.vr_explicit(); }
    void Reader::set_vr_explicit(bool vr_explicit) { m_raw_reader.set_vr_explicit(vr_explicit); }

    bool Reader::swap() const { return m_raw_reader.swap(); }

    std::streampos Reader::tell() {
      return m_raw_reader.tell();
    }

    std::streampos Reader::eof_pos() const {
      return m_raw_reader.eof_pos();
    }

    void Reader::seek_pos(std::streampos pos) {
      m_raw_reader.seek_pos(pos);
    }

    void Reader::seek_delta(std::streampos delta) {
      m_raw_reader.seek_delta(delta);
    }

    bool Reader::eof() {
      return m_raw_reader.eof();
    }

    void Reader::rewind() {
      m_raw_reader.rewind();
    }

    std::shared_ptr<DataElement> Reader::read_data_element(std::shared_ptr<DataSet> parent) {
      if (!parent) {
        throw ReadingError("read_data_element() must receive parent DataSet");
      }

      m_formatter.indent() << "read_data_element"; m_formatter.newline();
      m_formatter.increase_indent();

      Tag tag;
      VR vr;
      std::shared_ptr<DataElement> element;

      if (!m_raw_reader.read_into(&tag)) throw ReadingError("Reader failed to read element tag");

      if (m_raw_reader.vr_explicit()) {
        // Read explicit VR
        if (!m_raw_reader.read_into(&vr)) throw ReadingError("Reader failed to read in explicit VR");
        element = std::make_shared<DataElement>(tag, vr, parent);

        // Here, we have a 2 byte buffer, and then 32 bit length field
        if (element->vr().needs_two_byte_padding()) {
          this->seek_delta(2);
          if (!m_raw_reader.read_into(&element->length())) throw ReadingError("Reader failed to read in explicit VR 4 byte length field");
        }
        // Here we have 16 bit length field
        else {
          uint16_t length;
          if (!m_raw_reader.read_into(&length)) throw ReadingError("Reader failed to read in explicit VR 2 byte length field");
          element->length() = length;
        }
      }
      else {
        // Implicit VR, get VR from dictionary
        element = std::make_shared<DataElement>(tag, parent, true);

        // and read 32 bit length field
        if (!m_raw_reader.read_into(&element->length())) throw ReadingError("Reader failed to read in implicit VR 4 byte length field");
      }

      if (!element->vr().valid()) throw ReadingError(std::string("Reader encountered invalid VR2: ") + element->vr().long_str() + ", tag: " + element->tag().str());

      if (!element->page()->allows_vr(element->vr()) && (!m_raw_reader.vr_explicit() || (m_raw_reader.vr_explicit() && !m_allow_any_explicit_vr))) {
        throw ReadingError(std::string("Reader page does not allow VR: ") + element->vr().long_str() + ", tag: " + element->tag().str() + " for " + element->page()->name());
      }

      if (!element->is_undefined_length() && this->tell() + (std::streampos)element->length() > m_raw_reader.eof_pos()) {
        throw ReadingError(
          "Reader encounted length too long to fit in file: tag=" +
          element->tag().str() +
          " length=" +
          std::to_string(element->length()) +
          " cur_pos=" +
          std::to_string(this->tell()) +
          " eof=" +
          std::to_string(this->eof_pos())
        );
      }

      m_formatter.indent() << "read_data_element: " << element->tag() << " " << element->vr() << " " << element->length(); m_formatter.newline();

      if (element->is_sequence() || (element->tag().is_private() && element->is_undefined_length())) {
        // Ensure we have SQ VR
        element->vr() = vr::SQ;

        if (element->is_undefined_length()) {
          this->read_data_element_undefined_sequence(element);
        }
        else {
          if (m_lazy_load) {
            element->set_value_field(shared_from_this(), this->tell());
            this->seek_delta(element->length());
          }
          else {
            this->read_data_element_finite_sequence(element);
          }
        }
      }
      else {
        if (element->is_undefined_length()) {
          this->read_data_element_value_field(element);
        }
        else {
          if (m_lazy_load) {
            element->set_value_field(shared_from_this(), this->tell());
            this->seek_delta(element->length());
          }
          else {
            this->read_data_element_value_field(element);
          }
        }
      }

      m_formatter.indent() << "read_data_element, returning: " << element->tag() << " " << element->vr() << " " << element->length(); m_formatter.newline();

      m_formatter.decrease_indent();
      return element;
    }

    void Reader::read_data_element_undefined_sequence(std::shared_ptr<DataElement> element) {
      m_formatter.indent() << "element is sequence with undefined length"; m_formatter.newline();
      while (!this->eof()) {
        // Check if reached end of sequence
        Tag tag;
        auto cur_pos = this->tell();

        if (!m_raw_reader.read_into(&tag)) throw ReadingError("Reader failed to read sequence tag in undefined length element");

        if (tag.is_sequence_delimitation_tag()) {
          m_formatter.indent() << "--END-- of element (sequence delimit)"; m_formatter.newline();
          DataElementHeader::length_type length;
          if (!m_raw_reader.read_into(&length) || length != 0) throw ReadingError("Reader failed to read sequence length in undefined length element");
          break;
        }

        // Not end yet, so jump back to non-end of sequence tag
        this->seek_pos(cur_pos);
        m_formatter.increase_indent();
        auto data_set = this->read_data_set(element);
        m_formatter.decrease_indent();
        if (data_set) element->data_sets().push_back(data_set);
      }
    }

    void Reader::read_data_element_finite_sequence(std::shared_ptr<DataElement> element) {
      m_formatter.indent() << "element is sequence has regular length"; m_formatter.newline();
      auto end_of_element = this->tell() + (std::streampos)element->length();

      while (this->tell() < end_of_element) {
        m_formatter.increase_indent();
        auto data_set = this->read_data_set(element);
        m_formatter.decrease_indent();
        if (data_set) element->data_sets().push_back(data_set);
      }
    }

    void Reader::read_data_element_value_field(std::shared_ptr<DataElement> element) {
      m_formatter.indent() << "element is not sequence"; m_formatter.newline();
      // Not sequence, read raw data in
      auto manipulator = vega::manipulator_for(*element);
      element->set_manipulator(manipulator);
      if (!manipulator->read_from(&m_raw_reader, element->length())) throw ReadingError("Reader encountered error reading from manipulator: '" + element->tag().str() + " " + element->vr().str() + "' (" + vega::to_string(Word{.u = element->vr().data().value}) + ") length=" + vega::to_string(element->length()));
    }

    std::shared_ptr<DataSet> Reader::read_data_set(std::shared_ptr<DataElement> parent) {
      m_formatter.indent() << "read_data_set"; m_formatter.newline();
      m_formatter.increase_indent();

      Tag item_tag;
      if (!m_raw_reader.read_into(&item_tag)) throw ReadingError("Reader failed to read in data set tag");

      // Next DataSet
      if (item_tag.is_item_tag()) {
        // Read the dataset in and return it
        std::shared_ptr<DataSet> data_set = std::make_shared<DataSet>(parent);
        if (!m_raw_reader.read_into(&data_set->length)) throw ReadingError("Reader failed to read in data set length");

        m_formatter.indent() << "DataSet item tag: " << item_tag << " " << data_set->length; m_formatter.newline();

        if (data_set->is_undefined_length()) {
          m_formatter.indent() << "Undefined length data set"; m_formatter.newline();
          while (!this->eof()) {
            // Check if reached end of item
            Tag tag;
            auto cur_pos = this->tell();

            if (!m_raw_reader.read_into(&tag)) throw ReadingError();

            if (tag.is_item_delimitation_tag()) {
              m_formatter.indent() << "--END-- of data sets (item delimit)"; m_formatter.newline();
              DataElementHeader::length_type length;
              if (!m_raw_reader.read_into(&length) || length != 0) throw ReadingError("Reader failed to read element tag in undefined length data set");
              break;
            }

            // Not end yet, so jump back to non-end of sequence tag
            this->seek_pos(cur_pos);

            m_formatter.increase_indent();
            auto element = this->read_data_element(data_set);
            if (!element) throw ReadingError("Reader failed to read element in undefined length data set");
            m_formatter.decrease_indent();

            data_set->add_data_element(element);
          }
        }
        else {
          m_formatter.indent() << "Regular length data set"; m_formatter.newline();
          auto data_set_end = this->tell() + (std::streampos)data_set->length;

          while (this->tell() < data_set_end) {
            m_formatter.increase_indent();
            auto element = this->read_data_element(data_set);
            if (!element) throw ReadingError("Reader failed to read element in well-defined length data set");
            m_formatter.decrease_indent();

            data_set->add_data_element(element);
          }
        }

        m_formatter.indent() << "Returning dataset: " << item_tag << " " << data_set->length; m_formatter.newline();
        m_formatter.decrease_indent();
        return data_set;
      }
      // End of sequence of DataSets
      else if (item_tag.is_item_delimitation_tag()) {
        DataSet::length_type length;
        if (!m_raw_reader.read_into(&length) || length != 0) throw ReadingError("Reader failed to read in length of item delimitation tag");
        m_formatter.indent() << "DataSet (delim): " << item_tag << " " << length; m_formatter.newline();
        m_formatter.decrease_indent();
        return nullptr;
      }

      throw ReadingError("Reader unexpectedly reached end of read_data_set() method");
    }

    std::shared_ptr<DataElement> Reader::read_data_element(const Tag& tag, std::shared_ptr<DataSet> parent) {
      auto element = this->read_data_element(parent);
      if (!element || element->tag() != tag) throw ReadingError("Reader failed to read in element");
      return element;
    }
  }
}
