#pragma once

#include "../test_helper.h"

#include "vega/tag.h"
#include "vega/dictionary/dictionary.h"

using namespace vega;

TEST(DictionaryTest, simple_pages_test) {
  {
    const auto file_meta_information = vega::dictionary::Dictionary::instance().page_for(Tag{0x00020000});
    EXPECT_TRUE((bool)file_meta_information);
    EXPECT_EQ(file_meta_information->name(), std::string("FileMetaInformationGroupLength"));
    EXPECT_EQ(file_meta_information->multi_vr().vrs()[0], vr::UL);
  }

  {
    const auto seq_delimit_item = vega::dictionary::Dictionary::instance().page_for(Tag{0xfffee0dd});
    EXPECT_TRUE((bool)seq_delimit_item);
    EXPECT_EQ(seq_delimit_item->name(), std::string("SequenceDelimitationItem"));
  }

  {
    const auto does_not_exist = vega::dictionary::Dictionary::instance().page_for(Tag{0x7});
    EXPECT_FALSE((bool)does_not_exist);
  }
}

TEST(DictionaryTest, ranged_pages_test) {
  {
    const auto curve_dimensions = vega::dictionary::Dictionary::instance().page_for(Tag{0x50320005});
    EXPECT_TRUE((bool)curve_dimensions);
    EXPECT_EQ(curve_dimensions->name(), std::string("CurveDimensions"));

    EXPECT_EQ(vega::dictionary::Dictionary::instance().page_for(Tag{0x50020005})->name(), std::string("CurveDimensions"));
    EXPECT_EQ(vega::dictionary::Dictionary::instance().page_for(Tag{0x50FE0005})->name(), std::string("CurveDimensions"));

    EXPECT_FALSE((bool)vega::dictionary::Dictionary::instance().page_for(Tag{0x50000003}));
    EXPECT_FALSE((bool)vega::dictionary::Dictionary::instance().page_for(Tag{0x51000005}));
  }

  {
    const auto private_creator = vega::dictionary::Dictionary::instance().page_for(Tag{0x00090010});
    EXPECT_TRUE((bool)private_creator);
    EXPECT_EQ(private_creator->name(), std::string("PrivateCreator"));
  }
}

TEST(DictionaryTest, name_to_page_test) {
  {
    auto page = vega::dictionary::Dictionary::instance().page_for("CurveDimensions");
    EXPECT_TRUE(page->contains(Tag{0x50320005}));
  }

  {
    auto page = vega::dictionary::Dictionary::instance().page_for("NotAPage");
    EXPECT_FALSE((bool)page);
  }
}
