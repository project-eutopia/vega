#pragma once

#include <string>
#include <memory>

#include "vega/dicom/reader.h"
#include "vega/dicom/preamble.h"
#include "vega/dicom/file_meta.h"
#include "vega/dicom/data_set.h"
#include "vega/dicom/sop_class.h"
#include "vega/dicom/transfer_syntax.h"

namespace vega {
  namespace dicom {
    class File {
      private:
        std::shared_ptr<Preamble> m_preamble;
        std::shared_ptr<FileMeta> m_file_meta;
        std::shared_ptr<DataSet> m_data_set;

        static const std::vector<TransferSyntax>& trial_transfer_syntaxes_when_missing_file_meta();

      public:
        // Reading existing file in
        File(const std::string& file_name, bool allow_any_explicit_vr = false);
        File(std::shared_ptr<std::istream> is, bool allow_any_explicit_vr = false);

        // Building new file
        File(const SOPClass& sop_class);
        File(const SOPClass& sop_class, const UID& instance_uid);

        const std::shared_ptr<FileMeta>& file_meta() const;
        const std::shared_ptr<DataSet>& data_set() const;

        void write(const std::string& filename) const;
        void write(std::shared_ptr<std::ostream> os) const;
    };
  }
}
