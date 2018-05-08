#pragma once

#include "vega/range_type.h"

#include <string>
#include <memory>
#include <regex>
#include <ostream>

namespace vega {
  class RegexString;

  /// Date Time
  /* A concatenated date-time character string in the format: */
  /* YYYYMMDDHHMMSS.FFFFFF&ZZXX */
  /* The components of this string, from left to right, are YYYY = Year, MM = Month, DD = Day, HH = Hour (range "00" - "23"), MM = Minute (range "00" - "59"), SS = Second (range "00" - "60"). */
  /* FFFFFF = Fractional Second contains a fractional part of a second as small as 1 millionth of a second (range "000000" - "999999"). */
  /* &ZZXX is an optional suffix for offset from Coordinated Universal Time (UTC), where & = "+" or "-", and ZZ = Hours and XX = Minutes of offset. */
  /* The year, month, and day shall be interpreted as a date of the Gregorian calendar system. */
  /* A 24-hour clock is used. Midnight shall be represented by only "0000" since "2400" would violate the hour range. */
  /* The Fractional Second component, if present, shall contain 1 to 6 digits. If Fractional Second is unspecified the preceding "." shall not be included. The offset suffix, if present, shall contain 4 digits. The string may be padded with trailing SPACE characters. Leading and embedded spaces are not allowed. */
  /* A component that is omitted from the string is termed a null component. Trailing null components of Date Time indicate that the value is not precise to the precision of those components. The YYYY component shall not be null. Non-trailing null components are prohibited. The optional suffix is not considered as a component. */
  /* A Date Time value without the optional suffix is interpreted to be in the local time zone of the application creating the Data Element, unless explicitly specified by the Timezone Offset From UTC (0008,0201). */
  /* UTC offsets are calculated as "local time minus UTC". The offset for a Date Time value in UTC shall be +0000. */
  /* Note */
  /* The range of the offset is -1200 to +1400. The offset for United States Eastern Standard Time is -0500. The offset for Japan Standard Time is +0900. */
  /* The RFC 2822 use of -0000 as an offset to indicate local time is not allowed. */
  /* A Date Time value of 195308 means August 1953, not specific to particular day. A Date Time value of 19530827111300.0 means August 27, 1953, 11;13 a.m. accurate to 1/10th second. */
  /* The Second component may have a value of 60 only for a leap second. */
  /* The offset may be included regardless of null components; e.g., 2007-0500 is a legal value./ */

  /*! The Date Time common data type.  Indicates a concatenated date-time ASCII string in the
   format YYYYMMDDHHMMSS.FFFFFF&ZZZZ.  The components of this string, from left to right, are
   YYYY = Year, MM = Month, DD = Day, HH = Hour, MM = Minute, SS = Second, FFFFFF = Fraction
   Second, & = '+' or '-', and ZZZZ = Hours and Minutes of offset.  &ZZZZ is an optional suffix
   for plus/minus offset from Coordinated Universal Time.  A component that is omitted from the
   string is termed a null componennt.  Trailing null components of Date Time are ignored.
   Non-trailing null components are prohibited, given that the optional suffix is not
   considered as a component.

   26 bytes maximum (formatted string) */
  class DateTime {
    public:
      static const std::shared_ptr<const std::regex> SINGLE_REGEX;
      static const std::shared_ptr<const std::regex> RANGE_REGEX;

      // Can either be single DateTime
      std::shared_ptr<const RegexString> m_value;
      // Or a range of DateTimes
      std::shared_ptr<const DateTime> m_lower;
      std::shared_ptr<const DateTime> m_upper;

    public:
      DateTime();
      explicit DateTime(const std::string& s);
      DateTime(const std::shared_ptr<DateTime>& lower, const std::shared_ptr<DateTime>& upper);

      bool is_range() const;
      const std::shared_ptr<const DateTime>& lower() const;
      const std::shared_ptr<const DateTime>& upper() const;

      bool operator==(const DateTime& other) const;
      bool operator!=(const DateTime& other) const;

      std::string str() const;
      friend std::ostream& operator<<(std::ostream& os, const DateTime& date_time);
      friend std::istream& operator>>(std::istream& is, DateTime& date_time);

      void set_string(const std::string& s);

      template <typename T>
      friend void range_read(std::istream& is, T& t);

      static std::string read_single_string_from(std::istream& is);
  };
}
