#include "vega/manipulator.h"
#include "vega/dicom/data_set.h"
#include "vega/dicom/raw_reader.h"
#include "vega/dicom/raw_writer.h"
#include "vega/tag.h"
#include "vega/vega.h"

#include <algorithm>

namespace vega {
  namespace manipulators {
    std::shared_ptr<dicom::RawValue> EncapsulatedPixelDataManipulator::raw_value() {
      return nullptr;
    }

    std::string EncapsulatedPixelDataManipulator::str() const {
      return "";
    }

    bool EncapsulatedPixelDataManipulator::read_from(dicom::RawReader* reader, size_t num_bytes) {
      if (num_bytes != dicom::DataSet::UNDEFINED_LENGTH) throw vega::Exception("Encapsulated pixel data must have undefined length");

      Tag tag;
      dicom::DataSet::length_type length;

      // Read in offsets
      {
        if (!reader->read_into(&tag)) throw vega::Exception("EncapsulatedPixelDataManipulator could not read offset item tag");
        if (tag != Tag::ITEM) throw vega::Exception("EncapsulatedPixelDataManipulator should first read item tag, but found " + tag.str());

        if (!reader->read_into(&length)) throw vega::Exception("EncapsulatedPixelDataManipulator could not read offset item tag length");

        auto manipulator = std::make_shared<UL_Manipulator>();
        manipulator->read_from(reader, length);

        m_offsets.resize(manipulator->size());
        std::copy(manipulator->begin(), manipulator->end(), m_offsets.begin());
      }

      // Read in fragments
      while (true) {
        if (!reader->read_into(&tag)) throw vega::Exception("EncapsulatedPixelDataManipulator could not read tag");
        if (!reader->read_into(&length)) throw vega::Exception("EncapsulatedPixelDataManipulator could not read tag's length");

        if (tag == Tag::ITEM) {
          auto manipulator = std::make_shared<OB_Manipulator>();
          manipulator->read_from(reader, length);

          m_fragments.emplace_back();
          m_fragments.back().resize(manipulator->size());
          std::copy(manipulator->begin(), manipulator->end(), m_fragments.back().begin());
        }
        else if (tag == Tag::SEQ_DELIMITATION) {
          if (length != 0) {
            throw vega::Exception("Sequence delimitation item should have length 0");
          }
          break;
        }
        else {
          throw vega::Exception("Encountered unknown tag in EncapsulatedPixelDataManipulator: " + tag.str());
        }
      }

      if (m_offsets.size() > 0 && m_offsets.size() != m_fragments.size()) {
          throw vega::Exception("Mismatch between offset table and fragments");
      }

      return true;
    }

    size_t EncapsulatedPixelDataManipulator::write_to(dicom::RawWriter* writer) const {
      // Calculate offsets
      auto offsets_manipulator = std::make_shared<UL_Manipulator>();
      uint32_t cur_offset = 0;
      for (const auto& fragment : m_fragments) {
        offsets_manipulator->push_back(cur_offset);
        // One fragment is 4 byte tag, 4 byte length, then the contents
        cur_offset += 8 + fragment.size();
      }

      Tag tag;
      dicom::DataSet::length_type length;

      size_t count = 0;

      // Write offset table
      tag = Tag::ITEM;
      length = sizeof(UL_Manipulator::value_type) * offsets_manipulator->size();

      count += writer->write_from(tag);
      count += writer->write_from(length);
      count += offsets_manipulator->write_to(writer);

      // Write fragments
      for (const auto& fragment : m_fragments) {
        auto fragment_manipulator = std::make_shared<OB_Manipulator>(fragment.begin(), fragment.end());
        tag = Tag::ITEM;
        length = sizeof(OB_Manipulator::value_type) * fragment_manipulator->size();

        count += writer->write_from(tag);
        count += writer->write_from(length);
        count += fragment_manipulator->write_to(writer);
      }

      // Write trailing delimiter
      tag = Tag::SEQ_DELIMITATION;
      length = 0;

      count += writer->write_from(tag);
      count += writer->write_from(length);

      return count;
    }

    bool EncapsulatedPixelDataManipulator::operator==(const ValueManipulator& other) const {
      auto other_ptr = dynamic_cast<const EncapsulatedPixelDataManipulator*>(&other);
      if (!other_ptr) return false;

      if (m_fragments.size() != other_ptr->m_fragments.size()) return false;

      for (size_t i = 0; i < m_fragments.size(); ++i) {
        if (m_fragments[i].size() != other_ptr->m_fragments[i].size()) return false;
        if (!std::equal(m_fragments[i].begin(), m_fragments[i].end(), other_ptr->m_fragments[i].begin())) return false;
      }

      return true;
    }

    bool EncapsulatedPixelDataManipulator::operator!=(const ValueManipulator& other) const {
      return !(*this == other);
    }

    void EncapsulatedPixelDataManipulator::json(Formatter& formatter) const {
      // FIXME
      formatter << "\"encapsulated pixel data\"";
    }

    void EncapsulatedPixelDataManipulator::from_json(std::stringstream& json_string) {
      // FIXME
    }
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::EncapsulatedPixelDataManipulator>(VR::value_type value) {
      return manipulators::EncapsulatedPixelDataManipulator::allows_vr(VR{VR::Data{.value = value}});
    }
  }
}
