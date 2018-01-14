#pragma once

#include <fstream>
#include <memory>
#include <map>

#include "vega/vega.h"
#include "vega/dicom/reader.h"
#include "vega/dicom/transfer_syntax.h"
#include "vega/dicom/sop_class.h"
#include "vega/dicom/data_set.h"
#include "vega/dictionary/dictionary.h"

namespace vega {
  namespace dicom {
    class Writer;

    class FileMeta {
      private:
        static const Tag FileMetaInformationGroupLength;
        static const Tag TransferSyntaxUID;

        std::shared_ptr<DataSet> m_data_set;

        TransferSyntax m_transfer_syntax;
        SOPClass m_sop_class;
        UID m_instance_uid;

      public:
        class InvalidFileMeta : public vega::Exception { using vega::Exception::Exception; };

        // Building new FileMeta
        // Blank file meta
        FileMeta();
        // Use random SOP instance UID
        FileMeta(const SOPClass& sop_class);
        FileMeta(const SOPClass& sop_class, const UID& instance_uid);
        // Reading existing FileMeta
        FileMeta(Reader& reader);

        const TransferSyntax& transfer_syntax() const;
        void set_transfer_syntax(const TransferSyntax& other);

        Endian transfer_syntax_endian() const;
        bool transfer_syntax_vr_explicit() const;
        std::shared_ptr<const DataSet> data_set() const;

        const SOPClass& sop_class() const;
        const UID& instance_uid() const;

        bool present() const;

        size_t write(std::shared_ptr<Writer> writer);

      private:
        void read(Reader& reader);
        void fill_defaults(const SOPClass& sop_class, const UID& instance_uid);
    };
  }
}
