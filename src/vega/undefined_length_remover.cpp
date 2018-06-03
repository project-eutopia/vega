#include "vega/undefined_length_remover.h"

#include "vega/dicom/file.h"
#include "vega/dicom/data_set.h"
#include "vega/dicom/data_element.h"
#include "vega/visitor.h"

namespace vega {
  UndefinedLengthRemover::UndefinedLengthRemover() {}

  void UndefinedLengthRemover::remove_undefined_lengths(dicom::File& file) const {
    remove_undefined_lengths(*file.data_set());
  }

  void UndefinedLengthRemover::remove_undefined_lengths(dicom::DataSet& data_set) const {
    Visitor v {
      [this](dicom::DataElement& data_element) -> bool {
        return this->remove_undefined_lengths(data_element);
      }
    };

    v.visit(data_set);
  }

  bool UndefinedLengthRemover::remove_undefined_lengths(dicom::DataElement& data_element) const {
    if (data_element.is_undefined_length()) {
      if (data_element.tag().is_private()) return true;

      data_element.length() = 0;
    }

    return false;
  }
}
