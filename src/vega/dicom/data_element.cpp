#include "vega/dicom/data_element.h"
#include "vega/dicom/data_set.h"
#include "vega/dicom/reader.h"
#include "vega/manipulator.h"
#include "vega/vega.h"

namespace vega {
  namespace dicom {
    DataElement::DataElement(std::shared_ptr<DataSet> parent)
      :
        m_header(),
        m_page(),
        m_parent(parent),
        m_data_sets(),
        m_reader(),
        m_start(),
        m_manipulator()
    {}

    DataElement::DataElement(const std::string& name, std::shared_ptr<DataSet> parent, bool implicit_vr)
      :
        m_header(),
        m_page(parent ? parent->page_for(name) : vega::dictionary::Dictionary::instance().page_for(name)),
        m_parent(parent),
        m_data_sets(),
        m_reader(),
        m_start(),
        m_manipulator()
    {
      if (!m_page) throw vega::Exception(std::string("Cannot find dictionary page with name ") + name);

      if (!implicit_vr && !m_page->multi_vr().single()) {
        throw vega::Exception(std::string("DataElement(string), must pass VR for ambiguous VR when set to explicit, name = ") + name);
      }

      if (!implicit_vr && !m_page->multi_vr().single()) {
        throw vega::Exception(std::string("Must supply explicit VR to DataElement with name = ") + name);
      }

      if (!m_page->tag_mask().is_single()) {
        throw vega::Exception(std::string("Cannot initialize DataElement with dictionary page name that has ambiguous tag: ") + name);
      }

      m_header = DataElementHeader{
        .tag = m_page->tag_mask().singular_tag(),
        .vr = (implicit_vr ? m_page->determine_implicit_vr() : m_page->multi_vr().to_single_vr()),
        .length = 0
      };
    }

    DataElement::DataElement(const Tag& tag, std::shared_ptr<DataSet> parent, bool implicit_vr)
      :
        m_header(),
        m_page(parent ? parent->page_for(tag) : vega::dictionary::Dictionary::instance().page_for(tag)),
        m_parent(parent),
        m_data_sets(),
        m_reader(),
        m_start(),
        m_manipulator()
    {
      if (!m_page) {
        throw vega::Exception(std::string("In DataElement(Tag), could not find dictionary page with tag = ") + tag.str());
      }

      if (!implicit_vr && !m_page->multi_vr().single()) {
        throw vega::Exception(std::string("DataElement(Tag), must pass VR for ambiguous VR when set to explicit, tag = ") + tag.str());
      }

      m_header = DataElementHeader{
        .tag = tag,
        .vr = (implicit_vr ? m_page->determine_implicit_vr() : m_page->multi_vr().to_single_vr()),
        .length = 0
      };
    }

    DataElement::DataElement(const Tag& tag, const VR& vr, std::shared_ptr<DataSet> parent)
      :
        m_header({.tag = tag, .vr = vr, .length = 0}),
        m_page(parent ? parent->page_for(this->tag()) : vega::dictionary::Dictionary::instance().page_for(this->tag())),
        m_parent(parent),
        m_data_sets(),
        m_reader(),
        m_start(),
        m_manipulator()
    {
      if (this->vr().is_combined_vr()) {
        throw vega::Exception(std::string("DataElement cannot be constructed with multi-VR of ") + this->vr().str());
      }
      if (!m_page) {
        throw vega::Exception(std::string("In DataElement(Tag, VR), could not find dictionary page with tag = ") + tag.str());
      }
      if (!m_page->allows_vr(this->vr())) {
        throw vega::Exception(std::string("In DataElement(Tag, VR), invalid VR of ") + vr.str() + std::string(" for tag ") + tag.str() + std::string(", ") + m_page->name());
      }
    }

    void DataElement::set_value_field(const std::shared_ptr<Reader>& reader, const std::streampos& start) {
      m_reader = reader;
      m_start = start;
    }

    const std::shared_ptr<const dictionary::Page>& DataElement::page() const { return m_page; }
    void DataElement::set_page(std::shared_ptr<const dictionary::Page> page) { m_page = page; }

    const DataElementHeader& DataElement::header() const { return m_header; }
    DataElementHeader& DataElement::header() { return m_header; }

    const Tag& DataElement::tag() const { return m_header.tag; }
    Tag& DataElement::tag() { return m_header.tag; }

    const VR& DataElement::vr() const { return m_header.vr; }
    VR& DataElement::vr() { return m_header.vr; }

    const DataElementHeader::length_type& DataElement::length() const { return m_header.length; }
    DataElementHeader::length_type& DataElement::length() { return m_header.length; }

    const std::weak_ptr<DataSet>& DataElement::parent() const { return m_parent; }
    std::weak_ptr<DataSet>& DataElement::parent() { return m_parent; }

    const std::vector<std::shared_ptr<DataSet>>& DataElement::data_sets() const { lazy_load(); return m_data_sets; }
    std::vector<std::shared_ptr<DataSet>>& DataElement::data_sets() { lazy_load(); return m_data_sets; }

    std::string DataElement::str() const {
      lazy_load();
      return m_manipulator->str();
    }

    bool DataElement::is_sequence() const {
      return this->vr().is_sequence() || (this->tag().is_private() && this->is_undefined_length());
    }
    bool DataElement::is_undefined_length() const { return m_header.is_undefined_length(); }

    bool DataElement::operator==(const DataElement& other) const {
      if (this->is_sequence()) {
        if (this->data_sets().size() != other.data_sets().size()) return false;

        for (size_t i = 0; i < this->data_sets().size(); ++i) {
          if (*this->data_sets()[i] != *other.data_sets()[i]) return false;
        }

        return true;
      }
      else {
        // Must have same tag and VR
        if (this->tag() != other.tag() || this->vr() != other.vr()) return false;

        // False if exactly one is blank
        if (bool(this->manipulator()) != bool(other.manipulator())) {
          return false;
        }

        // If blank, nothing to check
        if (!this->manipulator()) return true;

        return *this->manipulator() == *other.manipulator();
      }
    }

    bool DataElement::operator!=(const DataElement& other) const {
      return !(*this == other);
    }

    void DataElement::log(Formatter& formatter) const {
      formatter.indent() << this->tag() << " " << this->vr();
      if (this->page()) {
        formatter << " \"" << this->page()->name() << "\" VM=" << this->page()->vm();
      }
      formatter << " (len=" << this->length() << "): ";
      if (!this->is_sequence()) {
        if (this->tag() == Tag::PIXEL_DATA) {
          formatter << "Pixel Data (size " << this->length() << ")";
        }
        else {
          formatter << this->str();
        }
        formatter.newline();
      }
      else {
        formatter.newline();
        formatter.increase_indent();

        size_t i = 1;
        for (auto data_set : this->data_sets()) {
          formatter.indent() << "--- Data set " << i++ << "/" << this->data_sets().size() << " with " << data_set->size() << " elements ---";
          formatter.newline();
          data_set->log(formatter);
        }

        formatter.decrease_indent();
      }
    }

    void DataElement::json(Formatter& formatter) const {
      if (this->is_sequence()) {
        formatter << '[';
        for (unsigned i = 0; i < this->data_sets().size(); ++i) {
          const auto& data_set = this->data_sets()[i];
          data_set->json(formatter);
          if (i < this->data_sets().size() - 1) {
            formatter << ',';
          }
        }
        formatter << ']';
      }
      else {
        this->manipulator()->json(formatter);
      }
    }

    std::shared_ptr<DataElement> DataElement::from_json(std::stringstream& json_string, const Tag& tag, std::shared_ptr<DataSet> parent) {
      auto data_element = std::make_shared<DataElement>(tag, parent, true);

      if (data_element->is_sequence()) {
        char c;
        json_string >> c;
        if (c != '[') throw vega::Exception("Reading JSON data element, did not find '['");

        do {
          auto data_set = DataSet::from_json(json_string);
          data_element->data_sets().push_back(data_set);
          json_string >> c;
        }
        while(c == ',');

        if (c != ']') throw vega::Exception("Reading JSON data element, did not find ']'");
      }
      else {
        std::shared_ptr<manipulators::ValueManipulator> manipulator;

        if (data_element->vr().is_combined_vr()) {
          auto general_manipulator = std::make_shared<manipulators::GeneralIntegerManipulator>();
          general_manipulator->from_json(json_string);
          manipulator = general_manipulator->to_specific_manipulator(data_element->vr());
        }
        else {
          manipulator = vega::manipulator_for(*data_element);
          manipulator->from_json(json_string);
        }

        data_element->set_manipulator(manipulator);
      }

      return data_element;
    }

    void DataElement::lazy_load() const {
      // Can definitely skip lazy loading if no reader
      if (!m_reader) return;
      std::lock_guard<std::mutex> lock(m_mutex);

      // Second return check because reader might have been set to nullptr before the mutex was locked
      if (!m_reader) return;
      auto reader = m_reader;
      m_reader = nullptr;

      auto current = reader->tell();
      reader->seek_pos(m_start);

      if (this->is_sequence()) {
        if (this->is_undefined_length()) {
          throw vega::Exception("Internal error: lazy_load encountered undefined length");
        }
        else {
          this->read_finite_sequence(reader);
        }
      }
      else {
        if (this->is_undefined_length()) {
          throw vega::Exception("Internal error: lazy_load encountered undefined length");
        }
        else {
          this->read_value_field(reader);
        }
      }

      reader->seek_pos(current);
      reader = nullptr;
    }

    void DataElement::read_finite_sequence(const std::shared_ptr<Reader>& reader) const {
      auto end_of_element = reader->tell() + (std::streampos)this->length();

      while (reader->tell() < end_of_element) {
        auto data_set = reader->read_data_set(std::const_pointer_cast<DataElement>(shared_from_this()));
        if (data_set) m_data_sets.push_back(data_set);
      }
    }

    void DataElement::read_value_field(const std::shared_ptr<Reader>& reader) const {
      // Not sequence, read raw data in
      m_manipulator = vega::manipulator_for(*this);
      this->validate_manipulator(*m_manipulator);

      if (!m_manipulator->read_from(&reader->raw_reader(), this->length())) throw Reader::ReadingError("Reader encountered error reading from manipulator: '" + this->tag().str() + " " + this->vr().str() + "' (" + vega::to_string(Word{.u = this->vr().data().value}) + ") length=" + vega::to_string(this->length()));
    }
  }
}
