#include "vega/manipulator.h"
#include "vega/dicom/data_element.h"

namespace vega {
  std::shared_ptr<manipulators::ValueManipulator> manipulator_for(const dicom::DataElement& element) {
    if (element.tag() == Tag::PIXEL_DATA) {
      if (element.is_undefined_length()) {
        return std::make_shared<EncapsulatedPixelData_Manipulator>();
      }
      else {
        return std::make_shared<PixelData_Manipulator>();
      }
    }

    if (element.vr() == vr::AE) {
      return std::make_shared<AE_Manipulator>();
    }
    else if (element.vr() == vr::AS) {
      return std::make_shared<AS_Manipulator>();
    }
    else if (element.vr() == vr::AT) {
      return std::make_shared<AT_Manipulator>();
    }
    else if (element.vr() == vr::CS) {
      return std::make_shared<CS_Manipulator>();
    }
    else if (element.vr() == vr::DA) {
      return std::make_shared<DA_Manipulator>();
    }
    else if (element.vr() == vr::DS) {
      return std::make_shared<DS_Manipulator>();
    }
    else if (element.vr() == vr::DT) {
      return std::make_shared<DT_Manipulator>();
    }
    else if (element.vr() == vr::FL) {
      return std::make_shared<FL_Manipulator>();
    }
    else if (element.vr() == vr::FD) {
      return std::make_shared<FD_Manipulator>();
    }
    else if (element.vr() == vr::IS) {
      return std::make_shared<IS_Manipulator>();
    }
    else if (element.vr() == vr::LO) {
      return std::make_shared<LO_Manipulator>();
    }
    else if (element.vr() == vr::LT) {
      return std::make_shared<LT_Manipulator>();
    }
    else if (element.vr() == vr::OB) {
      return std::make_shared<OB_Manipulator>();
    }
    else if (element.vr() == vr::OW) {
      return std::make_shared<OW_Manipulator>();
    }
    else if (element.vr() == vr::OF) {
      return std::make_shared<OF_Manipulator>();
    }
    else if (element.vr() == vr::OD) {
      return std::make_shared<OD_Manipulator>();
    }
    else if (element.vr() == vr::PN) {
      return std::make_shared<PN_Manipulator>();
    }
    else if (element.vr() == vr::SH) {
      return std::make_shared<SH_Manipulator>();
    }
    else if (element.vr() == vr::SL) {
      return std::make_shared<SL_Manipulator>();
    }
    else if (element.vr() == vr::SS) {
      return std::make_shared<SS_Manipulator>();
    }
    else if (element.vr() == vr::ST) {
      return std::make_shared<ST_Manipulator>();
    }
    else if (element.vr() == vr::TM) {
      return std::make_shared<TM_Manipulator>();
    }
    else if (element.vr() == vr::UI) {
      return std::make_shared<UI_Manipulator>();
    }
    else if (element.vr() == vr::UL) {
      return std::make_shared<UL_Manipulator>();
    }
    else if (element.vr() == vr::UN) {
      return std::make_shared<UN_Manipulator>();
    }
    else if (element.vr() == vr::US) {
      return std::make_shared<US_Manipulator>();
    }
    else if (element.vr() == vr::UT) {
      return std::make_shared<UT_Manipulator>();
    }
    else {
      // Unhandled!
      throw vega::Exception(std::string("Unhandled VR ") + element.vr().str() + std::string(" with tag ") + element.tag().str());
    }
  }
}
