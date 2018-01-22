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

    /*!
     * \class FileMeta
     * \brief A class which encapsulates the [file meta](http://dicom.nema.org/dicom/2013/output/chtml/part10/chapter_7.html) portion of a DICOM file.
     *
     * This class represents the [file meta](http://dicom.nema.org/dicom/2013/output/chtml/part10/chapter_7.html) part
     * of a DICOM file (the set of tags with group `0x0002`).
     * It has a `data_set()` method which contains the \link DataElement DataElements \endlink
     * that make up the file meta.
     */
    class FileMeta {
      private:
        static const Tag FileMetaInformationGroupLength;
        static const Tag TransferSyntaxUID;

        std::shared_ptr<DataSet> m_data_set;

        TransferSyntax m_transfer_syntax;
        SOPClass m_sop_class;
        UID m_media_storage_instance_uid;

      public:
        class InvalidFileMeta : public vega::Exception { using vega::Exception::Exception; };

        /**
         * Creates a blank FileMeta object.
         */
        FileMeta();
        /**
         * Creates a blank FileMeta object with the given \p sop_class and a random media storage SOP instance UID.
         *
         * \param sop_class represents the type of DICOM file and has tag `(0x0002,0x0002)`.
         */
        FileMeta(const SOPClass& sop_class);
        /**
         * Creates a blank FileMeta object with the given \p sop_class and \p media_storage_instance_uid.
         *
         * \param sop_class represents the type of DICOM file and has tag `(0x0002,0x0002)`.
         * \param media_storage_instance_uid is the media storage SOP instance UID with tag `(0x0002,0x0003)`.
         */
        FileMeta(const SOPClass& sop_class, const UID& media_storage_instance_uid);
        /**
         * Read existing FileMeta data from a Reader object currently positioned
         * at the start of the file meta (after the `"DICM"` characters).
         */
        FileMeta(Reader& reader);

        /// \return The TransferSyntax of this FileMeta.
        const TransferSyntax& transfer_syntax() const;
        /// \param The TransferSyntax to set for this FileMeta.
        void set_transfer_syntax(const TransferSyntax& other);

        /// \return The \link Endian Endianness \endlink of the DICOM file defined by
        /// the transfer syntax.
        Endian transfer_syntax_endian() const;
        /// \return a boolean which is true if the transfer syntax is VR explicit,
        ///         or false if VR implicit.
        bool transfer_syntax_vr_explicit() const;

        /// \return a DataSet which contains all the \link DataElement DataElements \endlink
        ///         that make up the file meta.
        std::shared_ptr<const DataSet> data_set() const;

        /// \return The media storage SOP class with tag `(0x0002,0x0002)` for this FileMeta.
        const SOPClass& sop_class() const;
        /// \return The media storage SOP instance UID with tag `(0x0002,0x0003)` for this FileMeta.
        const UID& media_storage_instance_uid() const;

        /// \return true if there exists some \link DataElement DataElements \endlink in this FileMeta.
        bool present() const;

        /**
         * Writes this FileMeta data (the DataSet) to the provided Writer class starting
         * from its current position.
         */
        size_t write(std::shared_ptr<Writer> writer);

      private:
        void read(Reader& reader);
        void fill_defaults(const SOPClass& sop_class, const UID& media_storage_instance_uid);
    };
  }
}
