#pragma once

#include "vega/vega.h"

#include <memory>

#include "vega/manipulators/application_entity_manipulator.h"
#include "vega/manipulators/age_string_manipulator.h"
#include "vega/manipulators/attribute_tag_manipulator.h"
#include "vega/manipulators/code_string_manipulator.h"
#include "vega/manipulators/date_manipulator.h"
#include "vega/manipulators/decimal_string_manipulator.h"
#include "vega/manipulators/date_time_manipulator.h"
#include "vega/manipulators/floating_point_manipulator.h"
#include "vega/manipulators/floating_point_double_manipulator.h"
#include "vega/manipulators/integer_string_manipulator.h"
#include "vega/manipulators/long_string_manipulator.h"
#include "vega/manipulators/long_text_manipulator.h"
#include "vega/manipulators/other_float_manipulator.h"
#include "vega/manipulators/other_double_manipulator.h"
#include "vega/manipulators/other_byte_manipulator.h"
#include "vega/manipulators/other_word_manipulator.h"
#include "vega/manipulators/other_long_manipulator.h"
#include "vega/manipulators/person_name_manipulator.h"
#include "vega/manipulators/short_string_manipulator.h"
#include "vega/manipulators/signed_long_manipulator.h"
#include "vega/manipulators/signed_short_manipulator.h"
#include "vega/manipulators/short_text_manipulator.h"
#include "vega/manipulators/time_manipulator.h"
#include "vega/manipulators/unique_identifier_manipulator.h"
#include "vega/manipulators/unsigned_long_manipulator.h"
#include "vega/manipulators/unknown_manipulator.h"
#include "vega/manipulators/unsigned_short_manipulator.h"
#include "vega/manipulators/unlimited_characters_manipulator.h"
#include "vega/manipulators/unlimited_text_manipulator.h"
#include "vega/manipulators/uri_manipulator.h"
#include "vega/manipulators/pixel_data_manipulator.h"
#include "vega/manipulators/encapsulated_pixel_data_manipulator.h"

namespace vega {
  typedef manipulators::ApplicationEntityManipulator     AE_Manipulator;
  typedef manipulators::AgeStringManipulator             AS_Manipulator;
  typedef manipulators::AttributeTagManipulator          AT_Manipulator;
  typedef manipulators::CodeStringManipulator            CS_Manipulator;
  typedef manipulators::DateManipulator                  DA_Manipulator;
  typedef manipulators::DecimalStringManipulator         DS_Manipulator;
  typedef manipulators::DateTimeManipulator              DT_Manipulator;
  typedef manipulators::FloatingPointManipulator         FL_Manipulator;
  typedef manipulators::FloatingPointDoubleManipulator   FD_Manipulator;
  typedef manipulators::IntegerStringManipulator         IS_Manipulator;
  typedef manipulators::LongStringManipulator            LO_Manipulator;
  typedef manipulators::LongTextManipulator              LT_Manipulator;
  typedef manipulators::OtherByteManipulator             OB_Manipulator;
  typedef manipulators::OtherDoubleManipulator           OD_Manipulator;
  typedef manipulators::OtherFloatManipulator            OF_Manipulator;
  typedef manipulators::OtherLongManipulator             OL_Manipulator;
  typedef manipulators::OtherWordManipulator             OW_Manipulator;
  typedef manipulators::PersonNameManipulator            PN_Manipulator;
  typedef manipulators::ShortStringManipulator           SH_Manipulator;
  typedef manipulators::SignedLongManipulator            SL_Manipulator;
  // No sequence manipulator
  typedef manipulators::SignedShortManipulator           SS_Manipulator;
  typedef manipulators::ShortTextManipulator             ST_Manipulator;
  typedef manipulators::TimeManipulator                  TM_Manipulator;
  typedef manipulators::UnlimitedCharactersManipulator   UC_Manipulator;
  typedef manipulators::UniqueIdentifierManipulator      UI_Manipulator;
  typedef manipulators::UnsignedLongManipulator          UL_Manipulator;
  typedef manipulators::UnknownManipulator               UN_Manipulator;
  typedef manipulators::URIManipulator                   UR_Manipulator;
  typedef manipulators::UnsignedShortManipulator         US_Manipulator;
  typedef manipulators::UnlimitedTextManipulator         UT_Manipulator;
  typedef manipulators::PixelDataManipulator             PixelData_Manipulator;
  typedef manipulators::EncapsulatedPixelDataManipulator EncapsulatedPixelData_Manipulator;

  namespace dicom {
    class DataElement;
  }

  std::shared_ptr<manipulators::ValueManipulator> manipulator_for(const dicom::DataElement& element);
}
