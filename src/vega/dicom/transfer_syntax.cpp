#include "vega/dicom/transfer_syntax.h"

#include "vega/vega.h"

namespace vega {
  namespace dicom {
    const std::vector<std::pair<std::string,UID>> TransferSyntax::NAME_UID_PAIRS = {
      {"Implicit VR Little Endian",                                                                 UID("1.2.840.10008.1.2")},
      {"Explicit VR Little Endian",                                                                 UID("1.2.840.10008.1.2.1")},
      {"Deflated Explicit VR Little Endian",                                                        UID("1.2.840.10008.1.2.1.99")},
      {"Explicit VR Big Endian",                                                                    UID("1.2.840.10008.1.2.2")},
      {"JPEG Baseline (Process 1)",                                                                 UID("1.2.840.10008.1.2.4.50")},
      {"JPEG Baseline (Processes 2 & 4)",                                                           UID("1.2.840.10008.1.2.4.51")},
      {"JPEG Lossless, Nonhierarchical (Processes 14)",                                             UID("1.2.840.10008.1.2.4.57")},
      {"JPEG Lossless, Nonhierarchical, First-Order Prediction (Processes 14 [Selection Value 1])", UID("1.2.840.10008.1.2.4.70")},
      {"JPEG-LS Lossless Image Compression",                                                        UID("1.2.840.10008.1.2.4.80")},
      {"JPEG-LS Lossy (Near-Lossless) Image Compression",                                           UID("1.2.840.10008.1.2.4.81")},
      {"JPEG 2000 Image Compression (Lossless Only)",                                               UID("1.2.840.10008.1.2.4.90")},
      {"JPEG 2000 Image Compression",                                                               UID("1.2.840.10008.1.2.4.91")},
      {"JPEG 2000 Part 2 Multicomponent Image Compression (Lossless Only)",                         UID("1.2.840.10008.1.2.4.92")},
      {"JPEG 2000 Part 2 Multicomponent Image Compression",                                         UID("1.2.840.10008.1.2.4.93")}
    };

    std::map<std::string,UID> TransferSyntax::generate_name_to_uid_map() {
      std::map<std::string,UID> m;

      for (const auto& pair : TransferSyntax::NAME_UID_PAIRS) {
        m.insert(pair);
      }

      return m;
    }

    std::map<std::string,std::string> TransferSyntax::generate_uid_to_name_map() {
      std::map<std::string,std::string> m;

      for (const auto& pair : TransferSyntax::NAME_UID_PAIRS) {
        auto pair2 = std::make_pair(pair.second.str(), pair.first);
        m.insert(pair2);
      }

      return m;
    }

    const std::map<std::string,UID>         TransferSyntax::NAME_TO_UID_MAP = TransferSyntax::generate_name_to_uid_map();
    const std::map<std::string,std::string> TransferSyntax::UID_TO_NAME_MAP = TransferSyntax::generate_uid_to_name_map();

    const TransferSyntax TransferSyntax::IMPLICIT_VR_LITTLE_ENDIAN{"Implicit VR Little Endian"};
    const TransferSyntax TransferSyntax::EXPLICIT_VR_LITTLE_ENDIAN{"Explicit VR Little Endian"};
    const TransferSyntax TransferSyntax::DEFLATED_EXPLICIT_VR_LITTLE_ENDIAN{"Deflated Explicit VR Little Endian"};
    const TransferSyntax TransferSyntax::EXPLICIT_VR_BIG_ENDIAN{"Explicit VR Big Endian"};

    TransferSyntax::TransferSyntax()
      : m_name(), m_uid()
    {}

    TransferSyntax::TransferSyntax(const std::string& name)
      : m_name(name)
    {
      auto it = TransferSyntax::NAME_TO_UID_MAP.find(m_name);
      if (it == TransferSyntax::NAME_TO_UID_MAP.end()) {
        // Maybe user passed in UID string...
        auto it2 = TransferSyntax::UID_TO_NAME_MAP.find(m_name);
        if (it2 == TransferSyntax::UID_TO_NAME_MAP.end()) {
          throw vega::Exception(std::string("Could not find transfer syntax with name: ") + m_name);
        }

        m_uid = UID(m_name);
        m_name = it2->second;
      }
      else {
        m_uid = it->second;
      }
    }

    TransferSyntax::TransferSyntax(const UID& uid)
      : m_uid(uid)
    {
      auto it = TransferSyntax::UID_TO_NAME_MAP.find(m_uid.str());
      if (it == TransferSyntax::UID_TO_NAME_MAP.end()) {
        throw vega::Exception(std::string("Could not find transfer syntax with uid: ") + m_uid.str());
      }

      m_name = it->second;
    }

    const std::string& TransferSyntax::name() const { return m_name; }
    const UID& TransferSyntax::uid() const { return m_uid; }
    bool TransferSyntax::is_deflate() const {
      return *this == TransferSyntax::DEFLATED_EXPLICIT_VR_LITTLE_ENDIAN;
    }

    bool TransferSyntax::is_explicit_vr() const {
      // Only one transfer syntax has implicit VR
      return *this != TransferSyntax::IMPLICIT_VR_LITTLE_ENDIAN;
    }

    Endian TransferSyntax::endianness() const {
      // Only one transfer syntax has big endian
      return *this == TransferSyntax::EXPLICIT_VR_BIG_ENDIAN ? Endian::BIG : Endian::LITTLE;
    }

    bool TransferSyntax::operator==(const TransferSyntax& other) const { return this->uid() == other.uid(); }
    bool TransferSyntax::operator!=(const TransferSyntax& other) const { return this->uid() != other.uid(); }
  }
}
