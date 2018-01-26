#include "vega/dicom/data_element.h"
#include "vega/dicom/data_set.h"
#include "vega/vega.h"

namespace vega {
  namespace dicom {
    DataElement::DataElement(std::shared_ptr<DataSet> parent)
      :
        m_header(),
        m_page(),
        m_parent(parent),
        m_data_sets(),
        m_manipulator()
    {}

    DataElement::DataElement(const std::string& name, std::shared_ptr<DataSet> parent)
      :
        m_header(),
        m_page(),
        m_parent(parent),
        m_data_sets(),
        m_manipulator()
    {
      auto page = vega::dictionary::Dictionary::instance().page_for(name);
      if (!page) throw vega::Exception(std::string("Cannot find dictionary page with name ") + name);

      if (!page->tag_mask().is_single()) throw vega::Exception(std::string("Cannot initialize DataElement with dictionary page name that has ambiguous tag: ") + name);

      m_page = page;
      m_header = DataElementHeader{
        .tag = page->tag_mask().singular_tag(),
        .vr = m_page->multi_vr().to_single_vr(),
        .length = 0
      };
    }

    DataElement::DataElement(const Tag& tag, std::shared_ptr<DataSet> parent)
      :
        m_header(),
        m_page(vega::dictionary::Dictionary::instance().page_for(tag)),
        m_parent(parent),
        m_data_sets(),
        m_manipulator()
    {
      m_header.tag = tag;
      if (!m_page) {
        throw vega::Exception(std::string("In DataElement(Tag), could not find dictionary page with tag = ") + tag.str());
      }
      if (!m_page->multi_vr().single()) {
        throw vega::Exception(std::string("Must supply explicit VR to DataElement constructor for ambiguous VR tag ") + this->tag().str());
      }
      m_header.vr = m_page->multi_vr().to_single_vr();
      m_header.length = 0;
    }

    DataElement::DataElement(const Tag& tag, const VR& vr, std::shared_ptr<DataSet> parent)
      :
        m_header({.tag = tag, .vr = vr, .length = 0}),
        m_page(vega::dictionary::Dictionary::instance().page_for(this->tag())),
        m_parent(parent),
        m_data_sets(),
        m_manipulator()
    {
      if (this->vr().is_combined_vr()) {
        throw vega::Exception(std::string("DataElement cannot be constructed with multi-VR of ") + this->vr().str());
      }
      if (!m_page) {
        throw vega::Exception(std::string("In DataElement(Tag, VR), could not find dictionary page with tag = ") + tag.str());
      }
      if (!m_page->multi_vr().contains(this->vr())) {
        throw vega::Exception(std::string("In DataElement(Tag, VR), invalid VR of ") + vr.str() + std::string(" for tag ") + tag.str());
      }
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

    const std::vector<std::shared_ptr<DataSet>>& DataElement::data_sets() const { return m_data_sets; }
    std::vector<std::shared_ptr<DataSet>>& DataElement::data_sets() { return m_data_sets; }

    std::string DataElement::str() const {
      return m_manipulator->str();
    }

    bool DataElement::is_sequence() const {
      return (this->tag().is_private() && this->is_undefined_length()) || this->vr().is_sequence();
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
      if (this->data_sets().empty()) {
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
  }
}
