#include "vega/dicom/file_meta.h"

#include "vega/dicom/writer.h"
#include "vega/dicom/data_element.h"
#include "vega/manipulator.h"

#include <cassert>

namespace vega {
  namespace dicom {
    const Tag FileMeta::FileMetaInformationGroupLength = Tag{0x2, 0x0};
    const Tag FileMeta::TransferSyntaxUID = Tag{0x2, 0x10};

    FileMeta::FileMeta()
      : m_transfer_syntax(TransferSyntax::EXPLICIT_VR_LITTLE_ENDIAN)
    {}

    FileMeta::FileMeta(const SOPClass& sop_class)
      : FileMeta(sop_class, UID::generate())
    {}

    FileMeta::FileMeta(const SOPClass& sop_class, const UID& media_storage_instance_uid)
      : m_data_set(std::make_shared<DataSet>())
    {
      this->fill_defaults(sop_class, media_storage_instance_uid);
    }

    FileMeta::FileMeta(Reader& reader)
      : m_data_set(std::make_shared<DataSet>())
    {
      // File meta information is written in little endian
      const Endian original_endian = reader.dicom_endian();
      reader.set_dicom_endianness(Endian::LITTLE);

      const bool original_vr_explicit = reader.vr_explicit();
      reader.set_vr_explicit(true);

      this->read(reader);

      reader.set_dicom_endianness(original_endian);
      reader.set_vr_explicit(original_vr_explicit);
    }

    const TransferSyntax& FileMeta::transfer_syntax() const { return m_transfer_syntax; }
    void FileMeta::set_transfer_syntax(const TransferSyntax& other) {
      m_transfer_syntax = other;
    }

    Endian FileMeta::transfer_syntax_endian() const { return m_transfer_syntax.endianness(); }
    bool FileMeta::transfer_syntax_vr_explicit() const { return m_transfer_syntax.is_explicit_vr(); }

    std::shared_ptr<const DataSet> FileMeta::data_set() const { return std::const_pointer_cast<const DataSet>(m_data_set); }

    const SOPClass& FileMeta::sop_class() const { return m_sop_class; }
    const UID& FileMeta::media_storage_instance_uid() const { return m_media_storage_instance_uid; }

    bool FileMeta::present() const { return bool(m_data_set); }

    size_t FileMeta::write(std::shared_ptr<Writer> writer) {
      if (!this->present()) return 0;

      // File meta information is written in little endian
      const Endian original_endian = writer->dicom_endian();
      writer->set_dicom_endianness(Endian::LITTLE);

      const bool original_vr_explicit = writer->vr_explicit();
      writer->set_vr_explicit(true);

      uint32_t length_of_file_meta_length_element = 0;
      uint32_t file_meta_bytes = 0;

      // Write each data element
      bool first = true;

      std::streampos file_meta_length_pos;
      std::streampos end_of_file_meta_pos;

      for (auto data_element : *m_data_set) {

        if (first) {
          first = false;

          length_of_file_meta_length_element += writer->write_element(data_element);
          assert(data_element->tag() == Tag(0x00020000));
          file_meta_length_pos = writer->tell() - std::streampos(sizeof(file_meta_bytes));
          assert(data_element->manipulator()->raw_value()->size() == sizeof(file_meta_bytes));
        }
        else {
          file_meta_bytes += writer->write_element(data_element);
        }
      }

      end_of_file_meta_pos = writer->tell();
      writer->seek_pos(file_meta_length_pos);
      writer->raw_writer().write_from(file_meta_bytes);
      writer->seek_pos(end_of_file_meta_pos);

      writer->set_dicom_endianness(original_endian);
      writer->set_vr_explicit(original_vr_explicit);

      return length_of_file_meta_length_element + file_meta_bytes;
    }

    void FileMeta::read(Reader& reader) {
      // Expect first data_element to be File Meta Information Group Length
      auto maybe_group_length = reader.read_data_element(m_data_set);

      if (maybe_group_length->tag() == FileMetaInformationGroupLength) {
        // Has group length
        auto group_length = maybe_group_length;
        auto manipulator = group_length->get_manipulator<UL_Manipulator>();
        std::streampos end_of_file_meta = reader.tell() + (std::streampos)(*manipulator)[0];
        m_data_set->add_data_element(group_length);

        // Read in rest of file meta elements
        while (reader.tell() < end_of_file_meta) {
          auto data_element = reader.read_data_element(m_data_set);
          if (!data_element) throw InvalidFileMeta("Unexpected error reading file meta");

          if (!data_element->tag().is_file_meta()) {
            throw InvalidFileMeta("Encountered non file-meta DataElement in file meta header");
          }

          m_data_set->add_data_element(data_element);
        }

        assert(reader.tell() == end_of_file_meta);
      }
      else {
        // Group length not present, but we would like to have one anyway for compliance so create
        auto actual_group_length = std::make_shared<dicom::DataElement>("FileMetaInformationGroupLength");
        auto manipulator = actual_group_length->get_manipulator<UL_Manipulator>();
        manipulator->push_back(0);
        m_data_set->add_data_element(actual_group_length);

        // Previously read in data_element still needs to be added
        m_data_set->add_data_element(maybe_group_length);

        // Read until no longer getting file meta elements
        while (!reader.eof()) {
          // Tentatively read in next tag
          std::streampos cur_pos = reader.tell();
          Tag temp_tag;
          reader.raw_reader().read_into(&temp_tag);
          reader.seek_pos(cur_pos);

          if (!temp_tag.is_file_meta()) {
            // Finished reading file meta
            break;
          }

          auto data_element = reader.read_data_element(m_data_set);
          if (!data_element) throw InvalidFileMeta("Unexpected error reading file meta");
          m_data_set->add_data_element(data_element);
        }
      }

      auto transfer_syntax_uid = m_data_set->data_element(TransferSyntaxUID);
      if (!transfer_syntax_uid) {
        throw InvalidFileMeta("Need TransferSyntaxUID element");
      }

      auto sop_class = m_data_set->data_element(SOPClass::TAG);
      if (!sop_class) {
        throw InvalidFileMeta("Need MediaStorageSOPClassUID element");
      }

      auto sop_class_manipulator = sop_class->get_manipulator<manipulators::UniqueIdentifierManipulator>();
      m_sop_class = SOPClass(sop_class_manipulator->uid());

      auto sop_instance = m_data_set->data_element("MediaStorageSOPInstanceUID");
      if (!sop_instance) {
        throw InvalidFileMeta("Need MediaStorageSOPInstanceUID element");
      }

      auto sop_instance_manipulator = sop_instance->get_manipulator<manipulators::UniqueIdentifierManipulator>();
      m_media_storage_instance_uid = sop_instance_manipulator->uid();

      this->set_transfer_syntax(TransferSyntax{UID{transfer_syntax_uid->str()}});
    }

    void FileMeta::fill_defaults(const SOPClass& sop_class, const UID& media_storage_instance_uid) {
      // File meta group length
      {
        auto data_element = std::make_shared<dicom::DataElement>("FileMetaInformationGroupLength");
        auto manipulator = data_element->get_manipulator<manipulators::UnsignedLongManipulator>();
        // Store dummy value of zero here -- only need this value when writing to file
        manipulator->push_back(0);
        m_data_set->add_data_element(data_element);
      }

      // File meta information version (0x00, 0x01)
      {
        auto data_element = std::make_shared<dicom::DataElement>("FileMetaInformationVersion");
        auto manipulator = data_element->get_manipulator<manipulators::OtherByteManipulator>();
        manipulator->push_back(Byte{.u=0});
        manipulator->push_back(Byte{.u=1});
        m_data_set->add_data_element(data_element);
      }

      // SOP class uid
      {
        auto data_element = std::make_shared<dicom::DataElement>("MediaStorageSOPClassUID");
        auto manipulator = data_element->get_manipulator<manipulators::UniqueIdentifierManipulator>();
        m_sop_class = sop_class;
        manipulator->uid() = m_sop_class.uid();
        m_data_set->add_data_element(data_element);
      }

      // SOP instance uid
      {
        auto data_element = std::make_shared<dicom::DataElement>("MediaStorageSOPInstanceUID");
        auto manipulator = data_element->get_manipulator<manipulators::UniqueIdentifierManipulator>();
        m_media_storage_instance_uid = media_storage_instance_uid;
        manipulator->uid() = m_media_storage_instance_uid;
        m_data_set->add_data_element(data_element);
      }

      // Transfer syntax we default to implicit VR little endian
      {
        auto data_element = std::make_shared<dicom::DataElement>("TransferSyntaxUID");
        auto manipulator = data_element->get_manipulator<manipulators::UniqueIdentifierManipulator>();
        this->set_transfer_syntax(TransferSyntax::IMPLICIT_VR_LITTLE_ENDIAN);
        manipulator->uid() = TransferSyntax::IMPLICIT_VR_LITTLE_ENDIAN.uid();
        m_data_set->add_data_element(data_element);
      }

      // Implementation class UID
      {
        auto data_element = std::make_shared<dicom::DataElement>("ImplementationClassUID");
        auto manipulator = data_element->get_manipulator<manipulators::UniqueIdentifierManipulator>();
        manipulator->uid() = UID::IMPLEMENTATION_CLASS_UID;
        m_data_set->add_data_element(data_element);
      }
    }
  }
}
