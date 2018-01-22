#include "vega/dicom/sop_class.h"

#include "vega/vega.h"
#include "vega/dictionary_data.h"

namespace vega {
  namespace dicom {
    const Tag SOPClass::TAG = vega::dictionary::MediaStorageSOPClassUID::tag;

    const std::vector<std::pair<std::string,UID>> SOPClass::NAME_UID_PAIRS = {
      {"Computed Radiography Image Storage",                                          UID("1.2.840.10008.5.1.4.1.1.1")},
      {"Digital X-Ray Image Storage - For Presentation",                              UID("1.2.840.10008.5.1.4.1.1.1.1")},
      {"Digital X-Ray Image Storage - For Processing",                                UID("1.2.840.10008.5.1.4.1.1.1.1.1")},
      {"Digital Mammography X-Ray Image Storage - For Presentation",                  UID("1.2.840.10008.5.1.4.1.1.1.2")},
      {"Digital Mammography X-Ray Image Storage - For Processing",                    UID("1.2.840.10008.5.1.4.1.1.1.2.1")},
      {"Digital Intra-Oral X-Ray Image Storage - For Presentation",                   UID("1.2.840.10008.5.1.4.1.1.1.3")},
      {"Digital Intra-Oral X-Ray Image Storage - For Processing",                     UID("1.2.840.10008.5.1.4.1.1.1.3.1")},
      {"CT Image Storage",                                                            UID("1.2.840.10008.5.1.4.1.1.2")},
      {"Enhanced CT Image Storage",                                                   UID("1.2.840.10008.5.1.4.1.1.2.1")},
      {"Legacy Converted Enhanced CT Image Storage",                                  UID("1.2.840.10008.5.1.4.1.1.2.2")},
      {"Ultrasound Multi-frame Image Storage (Retired)",                              UID("1.2.840.10008.5.1.4.1.1.3")},
      {"Ultrasound Multi-frame Image Storage",                                        UID("1.2.840.10008.5.1.4.1.1.3.1")},
      {"MR Image Storage",                                                            UID("1.2.840.10008.5.1.4.1.1.4")},
      {"Enhanced MR Image Storage",                                                   UID("1.2.840.10008.5.1.4.1.1.4.1")},
      {"MR Spectroscopy Storage",                                                     UID("1.2.840.10008.5.1.4.1.1.4.2")},
      {"Enhanced MR Color Image Storage",                                             UID("1.2.840.10008.5.1.4.1.1.4.3")},
      {"Legacy Converted Enhanced MR Image Storage",                                  UID("1.2.840.10008.5.1.4.1.1.4.4")},
      {"Ultrasound Image Storage (Retired)",                                          UID("1.2.840.10008.5.1.4.1.1.6")},
      {"Ultrasound Image Storage",                                                    UID("1.2.840.10008.5.1.4.1.1.6.1")},
      {"Enhanced US Volume Storage",                                                  UID("1.2.840.10008.5.1.4.1.1.6.2")},
      {"Secondary Capture Image Storage",                                             UID("1.2.840.10008.5.1.4.1.1.7")},
      {"Multi-frame Single Bit Secondary Capture Image Storage",                      UID("1.2.840.10008.5.1.4.1.1.7.1")},
      {"Multi-frame Grayscale Byte Secondary Capture Image Storage",                  UID("1.2.840.10008.5.1.4.1.1.7.2")},
      {"Multi-frame Grayscale Word Secondary Capture Image Storage",                  UID("1.2.840.10008.5.1.4.1.1.7.3")},
      {"Multi-frame True Color Secondary Capture Image Storage",                      UID("1.2.840.10008.5.1.4.1.1.7.4")},
      {"12-lead ECG Waveform Storage",                                                UID("1.2.840.10008.5.1.4.1.1.9.1.1")},
      {"General ECG Waveform Storage",                                                UID("1.2.840.10008.5.1.4.1.1.9.1.2")},
      {"Ambulatory ECG Waveform Storage",                                             UID("1.2.840.10008.5.1.4.1.1.9.1.3")},
      {"Hemodynamic Waveform Storage",                                                UID("1.2.840.10008.5.1.4.1.1.9.2.1")},
      {"Cardiac Electrophysiology Waveform Storage",                                  UID("1.2.840.10008.5.1.4.1.1.9.3.1")},
      {"Basic Voice Audio Waveform Storage",                                          UID("1.2.840.10008.5.1.4.1.1.9.4.1")},
      {"General Audio Waveform Storage",                                              UID("1.2.840.10008.5.1.4.1.1.9.4.2")},
      {"Arterial Pulse Waveform Storage",                                             UID("1.2.840.10008.5.1.4.1.1.9.5.1")},
      {"Respiratory Waveform Storage",                                                UID("1.2.840.10008.5.1.4.1.1.9.6.1")},
      {"Grayscale Softcopy Presentation State Storage",                               UID("1.2.840.10008.5.1.4.1.1.11.1")},
      {"Color Softcopy Presentation State Storage",                                   UID("1.2.840.10008.5.1.4.1.1.11.2")},
      {"Pseudo-Color Softcopy Presentation State Storage",                            UID("1.2.840.10008.5.1.4.1.1.11.3")},
      {"Blending Softcopy Presentation State Storage",                                UID("1.2.840.10008.5.1.4.1.1.11.4")},
      {"XA/XRF Grayscale Softcopy Presentation State Storage",                        UID("1.2.840.10008.5.1.4.1.1.11.5")},
      {"X-Ray Angiographic Image Storage",                                            UID("1.2.840.10008.5.1.4.1.1.12.1")},
      {"Enhanced XA Image Storage",                                                   UID("1.2.840.10008.5.1.4.1.1.12.1.1")},
      {"X-Ray Radiofluoroscopic Image Storage",                                       UID("1.2.840.10008.5.1.4.1.1.12.2")},
      {"Enhanced XRF Image Storage",                                                  UID("1.2.840.10008.5.1.4.1.1.12.2.1")},
      {"X-Ray Angiographic Bi-plane Image Storage (Retired)",                         UID("1.2.840.10008.5.1.4.1.1.12.3")},
      {"X-Ray 3D Angiographic Image Storage",                                         UID("1.2.840.10008.5.1.4.1.1.13.1.1")},
      {"X-Ray 3D Craniofacial Image Storage",                                         UID("1.2.840.10008.5.1.4.1.1.13.1.2")},
      {"Breast Tomosynthesis Image Storage",                                          UID("1.2.840.10008.5.1.4.1.1.13.1.3")},
      {"Intravascular Optical Coherence Tomography Image Storage - For Presentation", UID("1.2.840.10008.5.1.4.1.1.14.1")},
      {"Intravascular Optical Coherence Tomography Image Storage - For Processing",   UID("1.2.840.10008.5.1.4.1.1.14.2")},
      {"Nuclear Medicine Image Storage",                                              UID("1.2.840.10008.5.1.4.1.1.20")},
      {"Nuclear Medicine Image Storage (Retired)",                                    UID("1.2.840.10008.5.1.4.1.1.5")},
      {"Raw Data Storage",                                                            UID("1.2.840.10008.5.1.4.1.1.66")},
      {"Spatial Registration Storage",                                                UID("1.2.840.10008.5.1.4.1.1.66.1")},
      {"Spatial Fiducials Storage",                                                   UID("1.2.840.10008.5.1.4.1.1.66.2")},
      {"Deformable Spatial Registration Storage",                                     UID("1.2.840.10008.5.1.4.1.1.66.3")},
      {"Segmentation Storage",                                                        UID("1.2.840.10008.5.1.4.1.1.66.4")},
      {"Surface Segmentation Storage",                                                UID("1.2.840.10008.5.1.4.1.1.66.5")},
      {"Real World Value Mapping Storage",                                            UID("1.2.840.10008.5.1.4.1.1.67")},
      {"Surface Scan Mesh Storage",                                                   UID("1.2.840.10008.5.1.4.1.1.68.1")},
      {"Surface Scan Point Cloud Storage",                                            UID("1.2.840.10008.5.1.4.1.1.68.2")},
      {"VL Endoscopic Image Storage",                                                 UID("1.2.840.10008.5.1.4.1.1.77.1.1")},
      {"Video Endoscopic Image Storage",                                              UID("1.2.840.10008.5.1.4.1.1.77.1.1.1")},
      {"VL Microscopic Image Storage",                                                UID("1.2.840.10008.5.1.4.1.1.77.1.2")},
      {"Video Microscopic Image Storage",                                             UID("1.2.840.10008.5.1.4.1.1.77.1.2.1")},
      {"VL Slide-Coordinates Microscopic Image Storage",                              UID("1.2.840.10008.5.1.4.1.1.77.1.3")},
      {"VL Photographic Image Storage",                                               UID("1.2.840.10008.5.1.4.1.1.77.1.4")},
      {"Video Photographic Image Storage",                                            UID("1.2.840.10008.5.1.4.1.1.77.1.4.1")},
      {"Ophthalmic Photography 8 Bit Image Storage",                                  UID("1.2.840.10008.5.1.4.1.1.77.1.5.1")},
      {"Ophthalmic Photography 16 Bit Image Storage",                                 UID("1.2.840.10008.5.1.4.1.1.77.1.5.2")},
      {"Stereometric Relationship Storage",                                           UID("1.2.840.10008.5.1.4.1.1.77.1.5.3")},
      {"Ophthalmic Tomography Image Storage",                                         UID("1.2.840.10008.5.1.4.1.1.77.1.5.4")},
      {"VL Whole Slide Microscopy Image Storage",                                     UID("1.2.840.10008.5.1.4.1.1.77.1.6")},
      {"Lensometry Measurements Storage",                                             UID("1.2.840.10008.5.1.4.1.1.78.1")},
      {"Autorefraction Measurements Storage",                                         UID("1.2.840.10008.5.1.4.1.1.78.2")},
      {"Keratometry Measurements Storage",                                            UID("1.2.840.10008.5.1.4.1.1.78.3")},
      {"Subjective Refraction Measurements Storage",                                  UID("1.2.840.10008.5.1.4.1.1.78.4")},
      {"Visual Acuity Measurements Storage",                                          UID("1.2.840.10008.5.1.4.1.1.78.5")},
      {"Spectacle Prescription Report Storage",                                       UID("1.2.840.10008.5.1.4.1.1.78.6")},
      {"Ophthalmic Axial Measurements Storage",                                       UID("1.2.840.10008.5.1.4.1.1.78.7")},
      {"Intraocular Lens Calculations Storage",                                       UID("1.2.840.10008.5.1.4.1.1.78.8")},
      {"Macular Grid Thickness and Volume Report",                                    UID("1.2.840.10008.5.1.4.1.1.79.1")},
      {"Ophthalmic Visual Field Static Perimetry Measurements Storage",               UID("1.2.840.10008.5.1.4.1.1.80.1")},
      {"Ophthalmic Thickness Map Storage",                                            UID("1.2.840.10008.5.1.4.1.1.81.1")},
      {"Corneal Topography Map Storage",                                              UID("1.2.840.10008.5.1.4.1.1.82.1")},
      {"Basic Text SR",                                                               UID("1.2.840.10008.5.1.4.1.1.88.11")},
      {"Enhanced SR",                                                                 UID("1.2.840.10008.5.1.4.1.1.88.22")},
      {"Comprehensive SR",                                                            UID("1.2.840.10008.5.1.4.1.1.88.33")},
      {"Comprehensive 3D SR",                                                         UID("1.2.840.10008.5.1.4.1.1.88.34")},
      {"Procedure Log",                                                               UID("1.2.840.10008.5.1.4.1.1.88.40")},
      {"Mammography CAD SR",                                                          UID("1.2.840.10008.5.1.4.1.1.88.50")},
      {"Key Object Selection",                                                        UID("1.2.840.10008.5.1.4.1.1.88.59")},
      {"Chest CAD SR",                                                                UID("1.2.840.10008.5.1.4.1.1.88.65")},
      {"X-Ray Radiation Dose SR",                                                     UID("1.2.840.10008.5.1.4.1.1.88.67")},
      {"Colon CAD SR",                                                                UID("1.2.840.10008.5.1.4.1.1.88.69")},
      {"Implantation Plan SR Document Storage",                                       UID("1.2.840.10008.5.1.4.1.1.88.70")},
      {"Encapsulated PDF Storage",                                                    UID("1.2.840.10008.5.1.4.1.1.104.1")},
      {"Encapsulated CDA Storage",                                                    UID("1.2.840.10008.5.1.4.1.1.104.2")},
      {"Positron Emission Tomography Image Storage",                                  UID("1.2.840.10008.5.1.4.1.1.128")},
      {"Enhanced PET Image Storage",                                                  UID("1.2.840.10008.5.1.4.1.1.130")},
      {"Legacy Converted Enhanced PET Image Storage",                                 UID("1.2.840.10008.5.1.4.1.1.128.1")},
      {"Basic Structured Display Storage",                                            UID("1.2.840.10008.5.1.4.1.1.131")},
      {"RT Image Storage",                                                            UID("1.2.840.10008.5.1.4.1.1.481.1")},
      {"RT Dose Storage",                                                             UID("1.2.840.10008.5.1.4.1.1.481.2")},
      {"RT Structure Set Storage",                                                    UID("1.2.840.10008.5.1.4.1.1.481.3")},
      {"RT Beams Treatment Record Storage",                                           UID("1.2.840.10008.5.1.4.1.1.481.4")},
      {"RT Plan Storage",                                                             UID("1.2.840.10008.5.1.4.1.1.481.5")},
      {"RT Brachy Treatment Record Storage",                                          UID("1.2.840.10008.5.1.4.1.1.481.6")},
      {"RT Treatment Summary Record Storage",                                         UID("1.2.840.10008.5.1.4.1.1.481.7")},
      {"RT Ion Plan Storage",                                                         UID("1.2.840.10008.5.1.4.1.1.481.8")},
      {"RT Ion Beams Treatment Record Storage",                                       UID("1.2.840.10008.5.1.4.1.1.481.9")},
      {"RT Beams Delivery Instruction Storage",                                       UID("1.2.840.10008.5.1.4.34.7")},
      {"Generic Implant Template Storage",                                            UID("1.2.840.10008.5.1.4.43.1")},
      {"Implant Assembly Template Storage",                                           UID("1.2.840.10008.5.1.4.44.1")},
      {"Implant Template Group Storage",                                              UID("1.2.840.10008.5.1.4.45.1")}
    };

    std::map<std::string,UID> SOPClass::generate_name_to_uid_map() {
      std::map<std::string,UID> m;

      for (const auto& pair : SOPClass::NAME_UID_PAIRS) {
        m.insert(pair);
      }

      return m;
    }

    std::map<std::string,std::string> SOPClass::generate_uid_to_name_map() {
      std::map<std::string,std::string> m;

      for (const auto& pair : SOPClass::NAME_UID_PAIRS) {
        auto pair2 = std::make_pair(pair.second.str(), pair.first);
        m.insert(pair2);
      }

      return m;
    }

    const std::map<std::string,UID>         SOPClass::NAME_TO_UID_MAP = SOPClass::generate_name_to_uid_map();
    const std::map<std::string,std::string> SOPClass::UID_TO_NAME_MAP = SOPClass::generate_uid_to_name_map();

    SOPClass::SOPClass()
      : m_name(), m_uid()
    {}

    SOPClass::SOPClass(const std::string& sop_class_name)
      : m_name(sop_class_name)
    {
      auto it = SOPClass::NAME_TO_UID_MAP.find(m_name);
      if (it == SOPClass::NAME_TO_UID_MAP.end()) {
        // Maybe user passed in UID string...
        auto it2 = SOPClass::UID_TO_NAME_MAP.find(m_name);
        if (it2 == SOPClass::UID_TO_NAME_MAP.end()) {
          throw vega::Exception(std::string("Could not find SOP class with name: ") + m_name);
        }

        m_uid = UID(m_name);
        m_name = it2->second;
      }
      else {
        m_uid = it->second;
      }
    }

    SOPClass::SOPClass(const UID& sop_class_uid)
      : m_uid(sop_class_uid)
    {
      auto it = SOPClass::UID_TO_NAME_MAP.find(m_uid.str());
      if (it == SOPClass::UID_TO_NAME_MAP.end()) {
        m_name = "Unknown";
      }
      else {
        m_name = it->second;
      }
    }

    const std::string& SOPClass::name() const { return m_name; }
    const UID& SOPClass::uid() const { return m_uid; }
  }
}
