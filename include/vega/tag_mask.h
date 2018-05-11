#pragma once

#include "vega/vega.h"
#include "vega/tag.h"
#include "vega/math.h"

#include <tuple>
#include <algorithm>

namespace vega {
  class TagMask {
    private:
      Tag m_value_tag;
      Tag m_mask_tag;

    public:
      // Format (40xx,xxab)
      // => group_value   = 0x4000, group_mask   = 0xFF00
      // => element_value = 0x00ab, element_mask = 0x00FF
      explicit TagMask(const std::string& group_, const std::string& element_)
        :
          m_value_tag(0),
          m_mask_tag(0)
      {
        if (group_.size() != 4 || element_.size() != 4) {
          throw vega::Exception("Invalid group/element in TagMask: " + group_ + ", " + element_);
        }

        std::string group(group_);
        std::transform(group.begin(), group.end(), group.begin(), ::tolower);
        std::string element(element_);
        std::transform(element.begin(), element.end(), element.begin(), ::tolower);

        char c;
        uint8_t digit;

        // Group
        for (int i = 0; i < 4; ++i) {
          c = group[3-i];
          if (c != 'x') {
            m_mask_tag.group() |= (0xF << 4*i);

            if (c >= '0' && c <= '9') {
              digit = (c - '0');
            }
            else {
              digit = 10 + (c - 'a');
            }

            m_value_tag.group() |= (digit << 4*i);
          }
        }

        // Element
        for (int i = 0; i < 4; ++i) {
          c = element[3-i];
          if (c != 'x') {
            m_mask_tag.element() |= (0xF << 4*i);

            if (c >= '0' && c <= '9') {
              digit = (c - '0');
            }
            else {
              digit = 10 + (c - 'a');
            }

            m_value_tag.element() |= (digit << 4*i);
          }
        }
      }

      explicit constexpr TagMask(const Tag::group_type& group_value, const Tag::group_type& group_mask, const Tag::element_type& element_value, const Tag::element_type& element_mask)
        :
          m_value_tag(group_value, element_value),
          m_mask_tag(group_mask, element_mask)
      {}

      const Tag& value_tag() const { return m_value_tag; }
      const Tag& mask_tag() const { return m_mask_tag; }

      bool is_private() const { return ((m_value_tag.group() & 1) == 1) && ((m_mask_tag.group() & 1) == 1); }

      bool is_single() const { return m_mask_tag.tag() == 0xFFFFFFFF; }
      const Tag& singular_tag() const {
        if (!this->is_single()) throw vega::Exception("Cannot get singular tag from TagMask with value and mask of " + this->value_tag().str() + " and " + this->mask_tag().str());
        return this->value_tag();
      }

      unsigned mask_popcount() const { return math::popcount(this->mask_tag().tag()); }

      bool operator==(const TagMask& other) const { return this->value_tag() == other.value_tag() && this->mask_tag() == other.mask_tag(); }
      bool operator!=(const TagMask& other) const { return !(*this == other); }
      bool operator<(const TagMask& other) const {
        return std::tie(this->value_tag(), this->mask_tag()) < std::tie(other.value_tag(), other.mask_tag());
      }

      // Group = 28xx =>
      //                 group_value = 2800
      //                 group_mask  = FF00
      //
      // So a group of 28ab would pass the check in this case because
      // 28ab & FF00 == 2800 which is the same as group_value
      //
      // Group = 1234 =>
      //                 group_value = 1234
      //                 group_mask  = FFFF
      //
      // Group = xxx0 =>
      //                 group_value = 0000
      //                 group_mask  = 000F
      bool contains(const Tag& tag) const {
        return (tag.tag() & this->mask_tag().tag()) == this->value_tag().tag();
      }
  };
}
