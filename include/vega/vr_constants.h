#pragma once

// AE: char, space padded, backslash delimited
//     Application Entity
//
// AS: char (4 bytes), space padding, backslash delimiter
//     Age String
//
// AT: two uint16_t, no padding, no delimiter
//     Attribute Tag
//
// CS: char (leading/trailing spaces non-significant), space padding, backslash delimiter
//     Code string
//
// DA: char (8 bytes), space padding, backslash delimiter
//     Date (YYYYMMDD)
//
// DS: char (max 16 bytes), space padding, backslash delimiter
//     Decimal string
//
// DT: char (max 26 bytes), space padding, backslash delimiter
//     Datetime (YYYYMMDDHHMMSS.FFFFFF&ZZXX)
//
// FL: float32, no padding, no delimiter
//     Floating point
//
// FD: double64, no padding, no delimiter
//     Floating point double
//
// IS: char (max 12), space padding, backslash delimiter
//     Integer string
//
// LO: char (max 64), space padding, backslash delimiter
//     Long string
//
// LT: char (max 10240, allow backslash), space padding, no delimiter (VM == 1)
//     Long text
//
// OB: bytes, null padding, no delimiter necessary
//     Other byte string
//
// OD: double64, no padding, no delimiter
//     Other double string
//
// OF: float32, no padding, no delimiter
//     Other float string
//
// OL: 4 byte, no padding, no delimiter
//     Other long
//
// OW: 16bit words, no padding, no delimiter
//     Other word string
//
// PN: char (max 64), space padding, backslash delimiter
//     Person name
//
// SH: char (max 16 leading/trailing space insignificant), space padding, backslash delimiter
//     Short string
//
// SL: int32_t, no padding, no delimiter
//     Signed long
//
// SQ: DataSets, no padding, no delimiter
//     Sequence
//
// SS: int16_t, no padding, no delimiter
//     Signed short
//
// ST: char (max 1024, allow backslash, trailing space insignificant), space padding, no delimiter (VM == 1)
//     Short text
//
// TM: char (max 16), space padding, backslash delimiter
//     Time (HHMMSS.FFFFFF)
//
// UC: char (max 2^32-2), space padding, backslash delimiter
//     Unlimited Characters
//
// UI: char (max 64), null padding, backslash delimiter
//     Unique Identifier
//
// UL: uint32_t, no padding, no delimiter
//     Unsigned long
//
// UN: bytes, no padding, no delimiter
//     Unknown
//
// UR: char (2^32-2), space padding, no delimiter (VM == 1)
//     Universal Resource Identifier
//
// US: uint16_t, no padding, no delimiter
//     Unsigned short
//
// UT: char (unlimited), space padding, no delimiter (VM == 1)
//     Unlimited text
//
// ox: OB or OW
// xs: SS or US
// xw: SS or US or OW
// uw: US or OW

namespace vega {
  namespace vr {
    VR parse_vr_string(std::string s);

    /// Application Entity
    /*! A string of characters that identifies an Application Entity with leading and trailer
     spaces (0x20) being non-significant.  A value consisting soley of spaces shall not be used.

     16 bytes maximum (string) (padded by space) */
    const VR::Data AE_DATA = {.characters = {'A', 'E'}};
    const VR::value_type AE_VALUE = AE_DATA.value;
    const VR AE{AE_VALUE};

    /// Age String
    /*!
     A string of characters with one of the following formats - nnnD, nnnW, nnnM, nnnY; where nnn
     shall contain the number of days for D, weeks for W, months for M, or years for Y.

     Example:  "018M" would represent an age of 18 months.

     4 bytes fixed (formatted string) */
    const VR::Data AS_DATA = {.characters = {'A', 'S'}};
    const VR::value_type AS_VALUE = AS_DATA.value;
    const VR AS{AS_VALUE};

    /// Attribute Tag
    /*! Ordered pair of 16-bit unsigned integers that is the value of a Data Element Tag.

     4 bytes fixed (pair UINT2) */
    const VR::Data AT_DATA = {.characters = {'A', 'T'}};
    const VR::value_type AT_VALUE = AT_DATA.value;
    const VR AT{AT_VALUE};

    /// Code String
    /*! A string of characters with leading or trailing spaces (0x20) being non-significant.

     16 bytes maximum (string) (padded by space) */
    const VR::Data CS_DATA = {.characters = {'C', 'S'}};
    const VR::value_type CS_VALUE = CS_DATA.value;
    const VR CS{CS_VALUE};

    /// Date
    /*! A string of characters of the format yyyymmdd; where yyyy shall contain year, mm shall
     contain month, and dd shall contain the day.  This conforms to the ANSI HISPP MSDS Date
     common data type.

     8 bytes fixed (formatted string) */
    const VR::Data DA_DATA = {.characters = {'D', 'A'}};
    const VR::value_type DA_VALUE = DA_DATA.value;
    const VR DA{DA_VALUE};

    /// Decimal String
    /*! A string of characters representing either a fixed point number or a floating point
     number.  A fixed point number shall contain only the characters 0-9 with an optional leading
     '+' or '-' and an optional '.' to mark the decimal point.  A floating point number shall
     be conveyed as defined in ANSI X3.9, with and 'E' or 'e' to indicate the start of the
     exponent.  Decimal Strings may be padded with leading or trailing spaces.  Embedded spaces
     are not allowed.

     16 bytes maximum (decimal string) (padded by space) */
    const VR::Data DS_DATA = {.characters = {'D', 'S'}};
    const VR::value_type DS_VALUE = DS_DATA.value;
    const VR DS{DS_VALUE};

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
    const VR::Data DT_DATA = {.characters = {'D', 'T'}};
    const VR::value_type DT_VALUE = DT_DATA.value;
    const VR DT{DT_VALUE};

    /// Floating Point Double
    /*! Double precision binary floating point number represented in IEEE 754: 1985 64-bit
     Floating Point Number Format.

     8 bytes fixed (double) */
    const VR::Data FD_DATA = {.characters = {'F', 'D'}};
    const VR::value_type FD_VALUE = FD_DATA.value;
    const VR FD{FD_VALUE};

    /// Floating Point Single
    /*! Single precision binary floating point number represented in IEEE 754: 1985 32-bit
     Floating Point Number Format.

     4 bytes fixed (float) */
    const VR::Data FL_DATA = {.characters = {'F', 'L'}};
    const VR::value_type FL_VALUE = FL_DATA.value;
    const VR FL{FL_VALUE};

    /// Integer String
    /*! A string of characters representing an Integer in base-10 (decimal), shall contain only
     the characters 0-9, with an optional leading '+' or '-'.  It may be padded with leading and/or
     trailing spaces.  Embedded spaces are not allowed.  The integer shall be in the range: (-2^31)
     to (2^31 - 1) inclusively.

     12 bytes maximum (integer string) (padded by space) */
    const VR::Data IS_DATA = {.characters = {'I', 'S'}};
    const VR::value_type IS_VALUE = IS_DATA.value;
    const VR IS{IS_VALUE};

    /// Long String
    /*! A character string that may be padded with leading and/or trailing spaces.  The character
     backslash '\\' shall not be present, as it is used as the delimiter between values in
     multiple valued data elements.  The string shall not have Control Characters except
     for ESC.

     64 chars maximum (string) (padded by space) */
    const VR::Data LO_DATA = {.characters = {'L', 'O'}};
    const VR::value_type LO_VALUE = LO_DATA.value;
    const VR LO{LO_VALUE};

    /// Long Text
    /*! A character string that may contain one or more paragraphs.  It may contain the Graphic
     Character set and the Control Characters, CR, LF, FF, and ESC.  It may be padded with trailing
     spaces, which may be ignored, but leading spaces are considered to be significant.  Data
     Elements with this VR shall not be multi-valued and therefore character backslash '\\'
     may be used.

     10240 chars maximum (string) (padded by space) */
    const VR::Data LT_DATA = {.characters = {'L', 'T'}};
    const VR::value_type LT_VALUE = LT_DATA.value;
    const VR LT{LT_VALUE};

    /// Other Byte String
    /*! A string of bytes where the encoding of the contents is specified by the negotiated
     Transfer Syntax.  The string of bytes shall be padded with a single trailing NULL byte
     when necessary to achieve even length.

     see Transfer Syntax definition (bytes - use hex) (padded by NULL) */
    const VR::Data OB_DATA = {.characters = {'O', 'B'}};
    const VR::value_type OB_VALUE = OB_DATA.value;
    const VR OB{OB_VALUE};

    /// Other Double String
    /*! A string of 64-bit IEEE 754:1985 floating point words. OD is a VR that requires byte swapping
        within each 64-bit word when changing between Little Endian and Big Endian byte ordering */
    const VR::Data OD_DATA = {.characters = {'O', 'D'}};
    const VR::value_type OD_VALUE = OD_DATA.value;
    const VR OD{OD_VALUE};

    /* OL */

    /* Other Long */
    /* A stream of 32-bit words where the encoding of the contents is specified by the negotiated Transfer Syntax. OL is a VR that requires byte swapping within each word when changing byte ordering (see Section 7.3). */
    const VR::Data OL_DATA = {.characters = {'O', 'L'}};
    const VR::value_type OL_VALUE = OL_DATA.value;
    const VR OL{OL_VALUE};

    /// Other Float String
    /*! A string of 32-bit IEEE 754: 1985 floating point words.

     2^32-4 maximum (4 byte words - hex) */
    const VR::Data OF_DATA = {.characters = {'O', 'F'}};
    const VR::value_type OF_VALUE = OF_DATA.value;
    const VR OF{OF_VALUE};

    /// Other Word String
    /*! A string of 16-bit words where the encoding of the contents is specified by the
     negotiated Transfer Syntax.

     see Transfer Syntax definition (2 byte words - hex) */
    const VR::Data OW_DATA = {.characters = {'O', 'W'}};
    const VR::value_type OW_VALUE = OW_DATA.value;
    const VR OW{OW_VALUE};

    /// Person Name
    /*! \todo Document person name VR

     64 chars maximum per component group (string) (padded by space) */
    const VR::Data PN_DATA = {.characters = {'P', 'N'}};
    const VR::value_type PN_VALUE = PN_DATA.value;
    const VR PN{PN_VALUE};

    /// Short String
    /*! A character string that may be padded with leading and/or trailer spaces.  The character
     backslash '\\' shall not be present, as it is used as the delimiter between values for
     multiple data elements.  The string shall not have Control Characters except ESC.

     16 chars maximum (string) (padded by space) */
    const VR::Data SH_DATA = {.characters = {'S', 'H'}};
    const VR::value_type SH_VALUE = SH_DATA.value;
    const VR SH{SH_VALUE};

    /// Signed Long
    /*! Signed binaryt integer 32 bits long in 2's complement form.  Represents an integer in
     the range (-2^31) to (2^31 - 1) inclusively.

     4 bytes maximum (INT4) */
    const VR::Data SL_DATA = {.characters = {'S', 'L'}};
    const VR::value_type SL_VALUE = SL_DATA.value;
    const VR SL{SL_VALUE};

    /// Sequence of Items
    /*! The value field is a sequence of zero or more items. */
    const VR::Data SQ_DATA = {.characters = {'S', 'Q'}};
    const VR::value_type SQ_VALUE = SQ_DATA.value;
    const VR SQ{SQ_VALUE};

    /// Signed Short
    /*! Signed binary integer 16 bits long in 2's complement form.  Represents an integer in
     the range (-2^15) to (2^15 - 1) inclusively.

     2 bytes fixed (INT2) */
    const VR::Data SS_DATA = {.characters = {'S', 'S'}};
    const VR::value_type SS_VALUE = SS_DATA.value;
    const VR SS{SS_VALUE};

    /// Short Text
    /*! A character string that may contain one or more paragraphs.  It may contain the Graphic
     Character set and the Control Characters, CR, LF, FF, and ESC.  It may be padded with
     trailing spaces, which may be ignored, but leading spaces are considered significant.
     Data Elements with this VR shall not be multi-valued and therefore character backslash
     '\\' may be used.

     1024 chars maximum (string) (padded by space) */
    const VR::Data ST_DATA = {.characters = {'S', 'T'}};
    const VR::value_type ST_VALUE = ST_DATA.value;
    const VR ST{ST_VALUE};

    /// Time
    /*! A string of characters of the format hhmmss.frac; where hh contains hours (range
     00 to 23), mm contains minutes (range 00 to 59), ss contains seconds (range 00 to 59),
     and frac contains a fractional part of a second as small as 1 millionth of a second
     (range 000000 to 999999).  A 24 hour clock is assumed.  Midnight can be represented
     by only 0000 since 2400 would violate the hour range.  The string may be padded with
     trailing spaces.  Leading and embedded spaces are not allowed.  One or more of the
     components mm, ss, or frac may be unspecified as long as every component to the right
     of an unspecified component is also unspecified.  If frac is unspecified the preceding
     '.' may not be included.

     16 bytes maximum (formatted string) (padded by space) */
    const VR::Data TM_DATA = {.characters = {'T', 'M'}};
    const VR::value_type TM_VALUE = TM_DATA.value;
    const VR TM{TM_VALUE};

    // Unlimited Characters
    // A character string that may be of unlimited length that may be padded with trailing spaces. The character code 5CH (the BACKSLASH "\" in ISO-IR 6) shall not be present, as it is used as the delimiter between values in multiple valued data elements. The string shall not have Control Characters except for ESC.
    // Default Character Repertoire and/or as defined by (0008,0005) excluding character code 5CH (the BACKSLASH "\" in ISO-IR 6), and all Control Characters except ESC when used for ISO 2022 escape sequences.
    const VR::Data UC_DATA = {.characters = {'U', 'C'}};
    const VR::value_type UC_VALUE = UC_DATA.value;
    const VR UC{UC_VALUE};

    /// Unique Identifier (UID)
    /*! A character string containing a UID that is used to uniquely identify a wide variety
     of items.  The UID is a series of numeric components separated by the period '.' character.
     If a %Value Field containing one or more UIDs is an odd number of bytes in length, the %Value
     Field shall be padded with a single trailing NULL character to ensure that the %Value Field
     is an even number of bytes in length.

     64 bytes maximum (string) (padded by NULL) */
    const VR::Data UI_DATA = {.characters = {'U', 'I'}};
    const VR::value_type UI_VALUE = UI_DATA.value;
    const VR UI{UI_VALUE};

    /// Unsigned Long
    /*! Unsigned binary integer 32 bits long.  Represents an integer in the range 0 to 2^32
     inclusively.

     4 bytes maximum (UINT4) */
    const VR::Data UL_DATA = {.characters = {'U', 'L'}};
    const VR::value_type UL_VALUE = UL_DATA.value;
    const VR UL{UL_VALUE};

    /// Unknown
    /*! A string of bytes where the encoding of the contents is unknown. Any length that is
     valid for any of the other DICOM %Value Representations is also valid for UN (string) */
    const VR::Data UN_DATA = {.characters = {'U', 'N'}};
    const VR::value_type UN_VALUE = UN_DATA.value;
    const VR UN{UN_VALUE};

    /* UR */

    /* Universal Resource Identifier or Universal Resource Locator (URI/URL) */
    /* A string of characters that identifies a URI or a URL as defined in [RFC3986]. Leading spaces are not allowed. Trailing spaces shall be ignored. Data Elements with this VR shall not be multi-valued. */

    /* Note */
    /* Both absolute and relative URIs are permitted. If the URI is relative, then it is relative to the base URI of the object within which it is contained. */
    const VR::Data UR_DATA = {.characters = {'U', 'R'}};
    const VR::value_type UR_VALUE = UR_DATA.value;
    const VR UR{UR_VALUE};

    /// Unsigned Short
    /*! Unsigned binary integer 16 bits long.  Represents an integer in the range 0 to 2^16
     inclusively.

     2 bytes fixed (UINT2) */
    const VR::Data US_DATA = {.characters = {'U', 'S'}};
    const VR::value_type US_VALUE = US_DATA.value;
    const VR US{US_VALUE};

    /// Unlimited Text
    /*! A character string that may contain one or more paragraphs.  It may contain the Graphic
     Character set and the Control Characters, CR, LF, FF, and ESC.  It may be padded with
     trailing spaces, which may be ignored, but leading spaces are considered to be significant.
     Data Elements with this VR shall not be multi-valued and there for character backslash
     '\\' may be used.

     2^32-2 limited by the size of the maximum unsigned integer representable in a 32 bit field
     minus one since 0xFFFFFFFF is reserved (padded by space) */
    const VR::Data UT_DATA = {.characters = {'U', 'T'}};
    const VR::value_type UT_VALUE = UT_DATA.value;
    const VR UT{UT_VALUE};

    // OB/OW => ox
    const VR::Data OX_DATA = {.characters = {'o', 'x'}};
    const VR::value_type OX_VALUE = OX_DATA.value;
    const VR OX{OX_VALUE};

    // US/SS
    const VR::Data XS_DATA = {.characters = {'x', 's'}};
    const VR::value_type XS_VALUE = XS_DATA.value;
    const VR XS{XS_VALUE};

    // US/SS/OW
    const VR::Data XW_DATA = {.characters = {'x', 'w'}};
    const VR::value_type XW_VALUE = XW_DATA.value;
    const VR XW{XW_VALUE};

    // US/OW
    const VR::Data UW_DATA = {.characters = {'u', 'w'}};
    const VR::value_type UW_VALUE = UW_DATA.value;
    const VR UW{UW_VALUE};
  }
}
