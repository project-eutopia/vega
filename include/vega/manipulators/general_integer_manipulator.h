#pragma once

#include "vega/types.h"
#include "vega/manipulators/fixed_size_element_manipulator.h"

namespace vega {
  namespace manipulators {
    // Long enough integer to fit signed/unsigned types up to 4 bytes in size
    class GeneralIntegerManipulator : public FixedSizeElementManipulator<int64_t> {
      using FixedSizeElementManipulator<int64_t>::FixedSizeElementManipulator;

      public:
        virtual bool is_valid_for(const VR& vr) const override;
        std::shared_ptr<ValueManipulator> to_specific_manipulator(VR& vr) const;

        template <typename T>
        T coerce_at(size_t i) const {
          return this->at(i);
        }

      private:
        template <typename M>
        std::shared_ptr<M> coerce_to_manipulator() const {
          auto manipulator = std::make_shared<M>();
          manipulator->reserve(this->size());

          for (size_t i = 0; i < this->size(); ++i) {
            manipulator->push_back(this->coerce_at<typename M::value_type>(i));
          }

          return manipulator;
        }
    };

    template <>
    Byte GeneralIntegerManipulator::coerce_at<Byte>(size_t i) const;

    template <>
    Word GeneralIntegerManipulator::coerce_at<Word>(size_t i) const;
  }

  namespace vr {
    template <>
    bool manipulator_is_valid_for<manipulators::GeneralIntegerManipulator>(VR::value_type value);
  }
}
