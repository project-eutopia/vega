#pragma once

#include "vega/vega.h"
#include "vega/uid.h"
#include <string>
#include <map>
#include <vector>

namespace vega {
  namespace dicom {
    class TransferSyntax {
      public:
        static const TransferSyntax IMPLICIT_VR_LITTLE_ENDIAN;
        static const TransferSyntax EXPLICIT_VR_LITTLE_ENDIAN;
        static const TransferSyntax DEFLATED_EXPLICIT_VR_LITTLE_ENDIAN;
        static const TransferSyntax EXPLICIT_VR_BIG_ENDIAN;

        TransferSyntax();
        explicit TransferSyntax(const std::string& name);
        explicit TransferSyntax(const UID& uid);

        const std::string& name() const;
        const UID& uid() const;

        bool is_explicit_vr() const;
        Endian endianness() const;
        bool is_deflate() const;

        bool operator==(const TransferSyntax& other) const;
        bool operator!=(const TransferSyntax& other) const;

      private:
        static std::map<std::string,UID> generate_name_to_uid_map();
        static std::map<std::string,std::string> generate_uid_to_name_map();

        std::string m_name;
        UID m_uid;

        // See source file for definition
        static const std::vector<std::pair<std::string,UID>> NAME_UID_PAIRS;
        static const std::map<std::string,UID> NAME_TO_UID_MAP;
        static const std::map<std::string,std::string> UID_TO_NAME_MAP;
    };
  }
}
