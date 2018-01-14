#pragma once

#include "test_helper.h"

#include "vega/regex_string.h"

using namespace vega;

TEST(RegexStringTest, regex_string_test) {
  {
    const std::shared_ptr<const std::regex> pattern = std::make_shared<const std::regex>("\\d{2}");
    vega::RegexString test_string{"12", pattern};
    EXPECT_EQ(test_string.str(), "12");
  }

  {
  //     Datetime (YYYYMMDDHHMMSS.FFFFFF&ZZXX)
    const std::shared_ptr<const std::regex> pattern = std::make_shared<const std::regex>(
      "\\d{4}((0[1-9]|1[0-2])((0[1-9]|[1-2]\\d|3[0-1])(([0-1]\\d|2[0-4])(([0-5]\\d|60)([0-5]\\d)?)?)?)?)?(\\.\\d{6})?([\\+\\-](0\\d|1[0-4])([0-5]\\d)?)?"
    );

    {vega::RegexString t("0123", pattern);}
    {vega::RegexString t("2020", pattern);}
    {vega::RegexString t("9999", pattern);}

    {vega::RegexString t("200001", pattern);}
    {vega::RegexString t("200012", pattern);}
    ASSERT_THROW(vega::RegexString t("200000", pattern), vega::Exception);
    ASSERT_THROW(vega::RegexString t("200013", pattern), vega::Exception);

    {vega::RegexString t("20000101", pattern);}
    {vega::RegexString t("20001231", pattern);}
    ASSERT_THROW(vega::RegexString t("20000600", pattern), vega::Exception);
    ASSERT_THROW(vega::RegexString t("20000632", pattern), vega::Exception);

    {vega::RegexString t("2000010100", pattern);}
    {vega::RegexString t("2000123124", pattern);}
    ASSERT_THROW(vega::RegexString t("2000060025", pattern), vega::Exception);

    {vega::RegexString t("200001010000", pattern);}
    {vega::RegexString t("200012312460", pattern);}
    ASSERT_THROW(vega::RegexString t("200006151261", pattern), vega::Exception);

    {vega::RegexString t("20000101120000", pattern);}
    {vega::RegexString t("20000101125959", pattern);}
    ASSERT_THROW(vega::RegexString t("20000101125960", pattern), vega::Exception);

    {vega::RegexString t("20201231235959.012345", pattern);}
    ASSERT_THROW(vega::RegexString t("20201231235959.01234", pattern), vega::Exception);

    {vega::RegexString t("20201231235959+1200", pattern);}
    {vega::RegexString t("20201231235959.012345+1200", pattern);}
  }
}
