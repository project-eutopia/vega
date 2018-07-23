#pragma once

#include "vega/randomizer.h"

#include <string>
#include <functional>

namespace vega {
  namespace dicom {
    class File;
    class DataSet;
    class DataElement;
  }

  /**
   * \class Anonymizer
   * \brief This class is used anonymize information contained in DICOM files.
   *
   * The algorithm emplyed in this code to anonymize files is as follows:
   * - Generate random character strings for elements of VR PN (person name)
   * - Remove private elements
   * - Blank all elements with group 0x0010 (e.g. patient sex, birth date)
   * - Remove all "curve" type elements with group 0x5000
   *
   * **NOTE**: anonymization of a DICOM file is complicated to accomplish completely in general,
   * so use discretion when using this feature to anonymize sensitive patient information.
   */
  class Anonymizer {
    private:
      Randomizer randomizer_;
      std::function<bool(dicom::DataElement&)> custom_anonymizer_;
      std::string patient_id_;

    public:
      /**
       * \param patient_id is the new PatientID to write into the DICOM file (blank means will be randomly generated)
       * \param custom_anonymizer is a custom function that can be used to add further anonymization.  It
       * will return true if the data element is to be removed, and false otherwise.
       */
      explicit Anonymizer(const std::string& patient_id = "", std::function<bool(dicom::DataElement&)> custom_anonymizer = nullptr);

      void set_patient_id(const std::string& patient_id);

      void anonymize(dicom::File& file);
      void anonymize(dicom::DataSet& data_set);

    private:
      bool anonymize(dicom::DataElement& data_element);
      void blank_data_element(dicom::DataElement& data_element) const;
      void set_deidentification_elements(dicom::DataSet& data_set) const;
      bool should_remove(const dicom::DataElement& data_element) const;

      Anonymizer(const Anonymizer&);
      Anonymizer& operator=(const Anonymizer&);
  };
}
