#pragma once

#include "vega/vega.h"
#include "vega/uid.h"
#include <string>
#include <map>
#include <vector>

namespace vega {
  namespace dicom {
    /**
     * \brief A class which encapsulates the DICOM transfer syntaxes.
     *
     * The transfer syntax is determined by the file meta DICOM tag `(0x0002,0x0010)`.
     * For a list of available syntaxes, [see here](https://www.dicomlibrary.com/dicom/transfer-syntax/).
     * A TransferSyntax can be initialized either by supplying a string representation of the
     * transfer syntax (e.g. `"Implicit VR Little Endian"`), or the UID directly
     * (e.g. `"1.2.840.10008.1.2"`).
     */
    class TransferSyntax {
      public:
        // See source file for definition
        // TODO: Include in Doxygen
        /// List of mappings from transfer syntax string to corresponding UID.
        static const std::vector<std::pair<std::string,UID>> NAME_UID_PAIRS;

        /// Implicit VR and little endian (`"1.2.840.10008.1.2"`)
        static const TransferSyntax IMPLICIT_VR_LITTLE_ENDIAN;
        /// Explicit VR and little endian (`"1.2.840.10008.1.2.1"`)
        static const TransferSyntax EXPLICIT_VR_LITTLE_ENDIAN;
        /// Explicit VR and little endian (`"1.2.840.10008.1.2.1.99"`)
        static const TransferSyntax DEFLATED_EXPLICIT_VR_LITTLE_ENDIAN;
        /// Explicit VR and big endian (`"1.2.840.10008.1.2.2"`)
        static const TransferSyntax EXPLICIT_VR_BIG_ENDIAN;

        /// Blank constructor
        TransferSyntax();
        /**
         * Builds transfer syntax from with the given \p name.
         *
         * Understood values include
         * - `"Implicit VR Little Endian"`,
         * - `"Explicit VR Little Endian"`,
         * - `"Deflated Explicit VR Little Endian"`,
         * - `"Explicit VR Big Endian"`.
         */
        explicit TransferSyntax(const std::string& name);
        /**
         * Builds transfer syntax with an given \p uid.
         */
        explicit TransferSyntax(const UID& uid);

        /// \return the name corresponding to this transfer syntax, e.g. `"Implicit VR Little Endian"`.
        const std::string& name() const;
        /// \return the UID for this transfer syntax.
        const UID& uid() const;

        /// \return `true` if the transfer syntax uses explicit VR.
        bool is_explicit_vr() const;
        /// \return the \link Endian Endianness \endlink specified by the transfer syntax.
        Endian endianness() const;
        /// \return `true` if the transfer syntax specifies that deflation should occur (zlib compression).
        bool is_deflate() const;

        bool operator==(const TransferSyntax& other) const;
        bool operator!=(const TransferSyntax& other) const;

      private:
        static std::map<std::string,UID> generate_name_to_uid_map();
        static std::map<std::string,std::string> generate_uid_to_name_map();

        std::string m_name;
        UID m_uid;

        static const std::map<std::string,UID> NAME_TO_UID_MAP;
        static const std::map<std::string,std::string> UID_TO_NAME_MAP;
    };
  }
}
