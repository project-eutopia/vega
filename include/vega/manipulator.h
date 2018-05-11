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
#include "vega/manipulators/general_integer_manipulator.h"
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

/**
 * \file
 * \brief For details, see documentation of namespace vega::manipulators.
 */

namespace vega {
  /**
   * \namespace vega::manipulators
   * \brief This namespace contains all the manipulators used to work with DataElement data.
   *
   * ### Manipulator concept
   *
   * A DICOM file is composed of a nested structure of many \link dicom::DataElement DataElements\endlink
   * and \link dicom::DataSet DataSets\endlink.  A DataElement can contain a wide variety of data determined
   * by its \link VR value representation\endlink.  For instance, data could be an array of signed 32
   * bit integers (vr::SL), or a string of characters that represent multiple integers separated by
   * backspaces (vr::IS).  The basic content is the same in each case, 32 bit signed integers, but
   * the way they are stored in the DataElement is very different.
   *
   * This library hides away the details of how the data is stored in the data elements through the
   * use of manipulators.  In the above case you would have the two manipulators SignedLongManipulator
   * and IntegerStringManipulator, both of which inherit from `std::vector<int32_t>` and so deal with
   * the data in a uniform way.  They know how the vector of integers should be stored in the DICOM
   * file, so you as a developer do not have to.
   *
   * ### Manipulators
   *
   * There are three main manipulators that all other manipulators inherit from:
   * - SingleStringManipulator:  A manipulator that wraps a single string.  This string
   *   can be read from and written to using the SingleStringManipulator::value() method.
   * - \link FixedSizeElementManipulator FixedSizeElementManipulator<T>\endlink:  A manipulator
   *   that wraps a vector of elements T which have a fixed byte size.  For example, raw arrays
   *   of 1, 2, 4, and 8 byte signed and unsigned integers, and floats and doubles are wrapped
   *   by this classes which inherit from this manipulator.
   * - \link PaddedStringManipulator PaddedStringManipulator<T>\endlink:  This manipulator
   *   is vector of objects that are stored in DICOM format using raw strings and delimited with
   *   the backslash character `\`.  For example type DecimalStringManipulator acts as a vector
   *   of DecimalString objects.
   *
   * ### Abbreviated versions
   *
   * In file vega/manipulator.h is defined many typedefs which give nice short names for
   * the complicated manipulator names here.  For instance, vega::manipulators::PersonNameManipulator
   * becomes just vega::PN_Manipulator.
   */

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
