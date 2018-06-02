#pragma once

#include "test_helper.h"

#include "vega/visitor.h"
#include "vega/dicom/data_element.h"
#include "vega/dicom/data_set.h"

using namespace vega;

TEST(VisitorTest, test_visiting) {
  dicom::DataSet data_set{};

  /* static constexpr const Tag tag {0x0008, 0x0040}; */
  // US
  auto data_set_type = data_set.new_element<dictionary::DataSetType>();

  /* static constexpr const Tag tag {0x0008, 0x0109}; */
  auto coding_schemes = data_set.new_element<dictionary::CodingSchemeResourcesSequence>();
  {
    coding_schemes->data_sets().push_back(std::make_shared<dicom::DataSet>());

    // US
    auto e = coding_schemes->data_sets().back()->new_element<dictionary::RecordInUseFlag>();
    e->manipulator()->push_back(3);
  }

  /* static constexpr const Tag tag {0x300A, 0x0306}; */
  // SS
  auto radiation_charge_state = data_set.new_element<dictionary::RadiationChargeState>();
  radiation_charge_state->manipulator()->push_back(5);

  {
    std::vector<Tag> tags;

    CVisitor v{
      [&tags](const dicom::DataElement& de) { tags.push_back(de.tag()); }
    };

    v.visit(data_set);

    EXPECT_EQ(tags.size(), 4);
    EXPECT_EQ(tags[0], dictionary::DataSetType::tag);
    EXPECT_EQ(tags[1], dictionary::CodingSchemeResourcesSequence::tag);
    EXPECT_EQ(tags[2], dictionary::RecordInUseFlag::tag);
    EXPECT_EQ(tags[3], dictionary::RadiationChargeState::tag);
  }

  {
    Visitor v{
      [](dicom::DataElement& de) -> bool {
        if (de.vr() == vr::US) {
          for (auto& val : *de.get_manipulator<US_Manipulator>()) {
            val *= 7;
          }
        }
        return false;
      }
    };

    v.visit(data_set);

    auto radiation_charge_state = data_set.element<dictionary::RadiationChargeState>();
    EXPECT_EQ(radiation_charge_state->manipulator()->at(0), 5);

    auto record_in_use_flag = data_set.element<dictionary::CodingSchemeResourcesSequence>()->data_sets().back()->element<dictionary::RecordInUseFlag>();
    // Has changed from 3 -> 21
    EXPECT_EQ(record_in_use_flag->manipulator()->at(0), 21);
  }

  {
    // Remove CodingSchemeResourcesSequence
    Visitor v{
      [](dicom::DataElement& de) -> bool {
        return de.tag() == dictionary::CodingSchemeResourcesSequence::tag;
      }
    };

    v.visit(data_set);

    EXPECT_EQ(data_set.size(), 2);
    EXPECT_NE(data_set.element<dictionary::DataSetType>(), nullptr);
    EXPECT_EQ(data_set.element<dictionary::CodingSchemeResourcesSequence>(), nullptr);
    EXPECT_NE(data_set.element<dictionary::RadiationChargeState>(), nullptr);
  }
}
