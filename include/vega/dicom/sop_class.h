#pragma once

#include <string>
#include <map>
#include <vector>

#include "vega/uid.h"
#include "vega/tag.h"

namespace vega {
  namespace dicom {
    /**
     * \brief A class which wraps the media storage SOP class.
     *
     * The media storage SOP class UID has Tag `(0x0002,0x0002)`.  This class is essentially
     * a wrapper for this UID, with the additional ability to define a SOPClass by name as
     * well ([see here](http://dicom.nema.org/dicom/2013/output/chtml/part04/sect_B.5.html),
     * for example e.g. `"RT Dose Storage"` will wrap the correct UID listed there).
     */
    class SOPClass {
      private:
        // See source file for definition
        static const std::vector<std::pair<std::string,UID>> NAME_UID_PAIRS;
        static const std::map<std::string,UID> NAME_TO_UID_MAP;
        static const std::map<std::string,std::string> UID_TO_NAME_MAP;

      public:
        static const Tag TAG;

        /// Creates a blank SOPClass.
        SOPClass();
        /// Creates a SOPClass with the given \p sop_class_name.
        /// For instance `"RT Dose Storage"` will wrap the UID `"1.2.840.10008.5.1.4.1.1.481.2"`.
        explicit SOPClass(const std::string& sop_class_name);
        /// Creates a SOPClass with the given \p sop_class_uid.
        explicit SOPClass(const UID& sop_class_uid);

        const std::string& name() const;
        const UID& uid() const;

      private:
        static std::map<std::string,UID> generate_name_to_uid_map();
        static std::map<std::string,std::string> generate_uid_to_name_map();

        std::string m_name;
        UID m_uid;
    };
  }
}
