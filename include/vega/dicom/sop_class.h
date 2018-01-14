#pragma once

#include "vega/uid.h"
#include <string>
#include <map>
#include <vector>

namespace vega {
  namespace dicom {
    class SOPClass {
      private:
        // See source file for definition
        static const std::vector<std::pair<std::string,UID>> NAME_UID_PAIRS;
        static const std::map<std::string,UID> NAME_TO_UID_MAP;
        static const std::map<std::string,std::string> UID_TO_NAME_MAP;

      public:
        SOPClass();
        explicit SOPClass(const std::string& sop_class_name);
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
