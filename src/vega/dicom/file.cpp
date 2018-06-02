#include "vega/dicom/file.h"
#include "vega/dicom/reader.h"
#include "vega/dicom/writer.h"
#include "vega/vega.h"

#include "vega/zlib.h"

#include <iostream>

namespace vega {
  namespace dicom {
    const std::vector<TransferSyntax>& File::trial_transfer_syntaxes_when_missing_file_meta() {
      static std::vector<TransferSyntax> v = {
        TransferSyntax::IMPLICIT_VR_LITTLE_ENDIAN,
        TransferSyntax::EXPLICIT_VR_LITTLE_ENDIAN,
        TransferSyntax::EXPLICIT_VR_BIG_ENDIAN,
        TransferSyntax::DEFLATED_EXPLICIT_VR_LITTLE_ENDIAN
      };
      return v;
    }

    File::File(const std::string& file_name, bool allow_any_explicit_vr, bool lazy_load)
      :
        File(std::make_shared<std::ifstream>(file_name, std::ifstream::binary), allow_any_explicit_vr, lazy_load)
    {}

    File::File(std::shared_ptr<std::istream> is, bool allow_any_explicit_vr, bool lazy_load)
      :
        m_data_set(std::make_shared<DataSet>())
    {
      if (!is || !*is) {
        throw vega::Exception("Invalid input stream");
      }
      auto reader = std::make_shared<Reader>(is, allow_any_explicit_vr, lazy_load);

      m_preamble = std::make_shared<Preamble>(*reader);

      auto start_of_file_meta = reader->tell();

      try {
        m_file_meta = std::make_shared<FileMeta>(*reader);
      }
      catch (const vega::Exception& ex) {
        std::cerr << "Warning, failure reading file meta so assuming blank: " << ex.what() << std::endl;
        reader->seek_pos(start_of_file_meta);
        m_file_meta = std::make_shared<FileMeta>();
      }

      reader->set_dicom_endianness(m_file_meta->transfer_syntax_endian());
      reader->set_vr_explicit(m_file_meta->transfer_syntax_vr_explicit());

      if (m_file_meta->present()) {
        auto current_reader = reader;

        // Here we need to inflate it
        if (m_file_meta->transfer_syntax().is_deflate()) {
          // Read in compressed data
          std::ostringstream ss;
          ss << is->rdbuf();
          std::string s = ss.str();

          auto decompressed_stream = RFC1951::inflate(s);
          current_reader = std::make_shared<Reader>(decompressed_stream);
        }

        while (!current_reader->eof()) {
          auto element = current_reader->read_data_element(m_data_set);
          if (!element) break;
          m_data_set->add_data_element(element);
        }

        if (!current_reader->eof()) throw vega::Exception("Failed to read in file");
      }

      else {
        for (const auto& ts : File::trial_transfer_syntaxes_when_missing_file_meta()) {
          auto start_of_data_elements = reader->tell();
          m_data_set = std::make_shared<DataSet>();

          m_file_meta->set_transfer_syntax(ts);
          reader->set_dicom_endianness(m_file_meta->transfer_syntax_endian());
          reader->set_vr_explicit(m_file_meta->transfer_syntax_vr_explicit());

          try {
            while (!reader->eof()) {
              auto element = reader->read_data_element(m_data_set);
              if (!element) break;
              m_data_set->add_data_element(element);
            }
          }
          catch (const vega::Exception& ex) {
            std::cerr << "Warning, trying to recover from error with transfer syntax " << ts.name() << ": " << ex.what() << std::endl;
            // Try transfer syntax
            reader->seek_pos(start_of_data_elements);
            continue;
          }

          if (!reader->eof()) throw vega::Exception("Failed to read in file");
          // Success!
          break;
        }
      }

      if (m_data_set->size() == 0) throw vega::Exception("Could not read any data elements from file");
    }

    File::File(const SOPClass& sop_class)
      : File(sop_class, UID::generate())
    {}

    File::File(const SOPClass& sop_class, const UID& media_storage_instance_uid)
      :
        m_preamble(std::make_shared<Preamble>()),
        m_file_meta(std::make_shared<FileMeta>(sop_class, media_storage_instance_uid)),
        m_data_set(std::make_shared<DataSet>())
    {}

    const std::shared_ptr<FileMeta>& File::file_meta() const {
      return m_file_meta;
    }

    const std::shared_ptr<DataSet>& File::data_set() const {
      return m_data_set;
    }

    void File::write() const {
      auto ss = std::make_shared<std::stringstream>();
      write(ss);
      std::cout << ss->str();
    }

    void File::write(const std::string& filename) const {
      std::shared_ptr<std::ofstream> ofs(std::make_shared<std::ofstream>(filename, std::ofstream::binary));
      if (!ofs || !*ofs) {
        throw vega::Exception(std::string("Could not open file: ") + filename);
      }

      this->write(ofs);
    }

    void File::write(std::shared_ptr<std::ostream> os) const {
      std::shared_ptr<Writer> writer = std::make_shared<Writer>(os);
      m_preamble->write(writer);
      m_file_meta->write(writer);

      writer->set_dicom_endianness(m_file_meta->transfer_syntax_endian());
      writer->set_vr_explicit(m_file_meta->transfer_syntax_vr_explicit());

      // Here we need to deflate it
      if (m_file_meta->transfer_syntax().is_deflate()) {
        auto oss = std::make_shared<std::ostringstream>();
        auto current_writer = std::make_shared<Writer>(oss);

        for (auto element : *m_data_set) {
          current_writer->write_element(element);
        }

        // Compress raw data
        std::string s = oss->str();
        auto compressed_stream = RFC1951::deflate(s);

        *os << compressed_stream->rdbuf();
      }
      else {
        for (auto element : *m_data_set) {
          writer->write_element(element);
        }
      }
    }

    void File::write_json() const {
      auto ss = std::make_shared<std::stringstream>();
      write_json(ss);
      std::cout << ss->str();
    }

    void File::write_json(const std::string& filename) const {
      std::shared_ptr<std::ofstream> ofs(std::make_shared<std::ofstream>(filename, std::ofstream::binary));
      if (!ofs || !*ofs) {
        throw vega::Exception(std::string("Could not open file: ") + filename);
      }

      this->write_json(ofs);
    }

    void File::write_json(std::shared_ptr<std::ostream> os) const {
      Formatter formatter(*os);
      m_data_set->json(formatter);
    }
  }
}
