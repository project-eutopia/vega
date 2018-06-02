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
    /*!
     * \class File
     * \brief A class which is used to read/build a DICOM file.
     *
     * This is the main I/O class for reading and writing of DICOM files.  It has constructors
     * that are used for reading in existing DICOM files into an internal DataSet, and also
     * constructors for building a blank DICOM file.
     */
    class File {
      private:
        std::shared_ptr<Preamble> m_preamble;
        std::shared_ptr<FileMeta> m_file_meta;
        std::shared_ptr<DataSet> m_data_set;

        static const std::vector<TransferSyntax>& trial_transfer_syntaxes_when_missing_file_meta();

      public:
        /**
         * \brief Reads in existing file with the given \p file_name.
         *
         * \param file_name the name of the file to be read in.
         * \param allow_any_explicit_vr will skip checking the dictionary to see if any element's
         *        VR is valid if the DICOM file has explicit VR.
         */
        File(const std::string& file_name, bool allow_any_explicit_vr = false, bool lazy_load = true);

        /**
         * \brief Reads in existing file from a user supplied input stream \p input.
         *
         * \param input is a pointer to the input stream to read from.  This allows reading
         *        in of DICOM files that are stored in both file streams and string streams (ostream).
         * \param allow_any_explicit_vr will skip checking the dictionary to see if any element's
         *        VR is valid if the DICOM file has explicit VR.
         */
        File(std::shared_ptr<std::istream> input, bool allow_any_explicit_vr = false, bool lazy_load = true);

        /**
         * \brief Builds a blank DICOM file.
         *
         * This constructor will automatically populate the FileMeta with the appropriate tags, and will generate a random SOP instance UID.
         *
         * \param sop_class is the SOPClass that defines the type of DICOM file to build.
         */
        File(const SOPClass& sop_class);

        /**
         * \brief Builds a blank DICOM file.
         *
         * This constructor will automatically populate the FileMeta with the appropriate tags, and will set the media storage SOP instance UID to the user supplied value \p media_storage_instance_uid.
         *
         * \param sop_class is the SOPClass that defines the type of DICOM file to build.
         * \param media_storage_instance_uid is the media storage SOP instance UID to be included in the FileMeta (with tag `(0x0002,0x0003)`).
         */
        File(const SOPClass& sop_class, const UID& media_storage_instance_uid);

        /// \return a pointer to the internal FileMeta object.
        const std::shared_ptr<FileMeta>& file_meta() const;
        /// \return a pointer to the internal DataSet object that is the root of the DICOM file, containing all the \link DataElement DataElements \endlink in the body of the DICOM file (not in the FileMeta).
        const std::shared_ptr<DataSet>& data_set() const;

        /**
         * Writes this DICOM file to STDOUT
         */
        void write() const;
        /**
         * Writes this DICOM file to the given \p filename.
         */
        void write(const std::string& filename) const;
        /**
         * Writes this DICOM file to the given output stream \p os.
         *
         * \param os is the output stream to write the file to (can be std::ofstream to write to
         *        file, or std::sstream to write to a string).
         */
        void write(std::shared_ptr<std::ostream> os) const;

        /**
         * Writes this DICOM file in JSON format to STDOUT
         */
        void write_json() const;
        /**
         * Writes this DICOM file in JSON format to the given \p filename.
         */
        void write_json(const std::string& filename) const;
        /**
         * Writes this DICOM file in JSON format to the given output stream \p os.
         *
         * \param os is the output stream to write the JSON file to (can be std::ofstream to write to
         *        file, or std::sstream to write to a string).
         */
        void write_json(std::shared_ptr<std::ostream> os) const;
    };
  }
}
