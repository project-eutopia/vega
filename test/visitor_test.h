#pragma once

#include "test_helper.h"

#include "vega/visitor.h"
#include "vega/dicom/data_element.h"
#include "vega/dicom/data_set.h"

using namespace vega;

TEST(VisitorTest, test_visiting) {
  dicom::DataSet data_set{};

  // VR: US, Tag: 0008,0040
  auto data_set_type = data_set.new_element<dictionary::DataSetType>();

  // VR: SQ, Tag: 0008,0109
  auto coding_schemes = data_set.new_element<dictionary::CodingSchemeResourcesSequence>();
  {
    coding_schemes->data_sets().push_back(std::make_shared<dicom::DataSet>());

    // VR: US, Tag: 0004,1410
    auto e = coding_schemes->data_sets().back()->new_element<dictionary::RecordInUseFlag>();
    e->manipulator()->push_back(3);
  }

  // VR: SS, Tag: 300A,0306
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
