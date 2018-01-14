#pragma once

#include <stdint.h>
#include <iostream>
#include <string>

namespace vega {
  class Tag {
    public:
      typedef uint16_t group_type;
      typedef uint16_t element_type;
      typedef uint32_t tag_type;

      static const tag_type ITEM_TAG;
      static const tag_type ITEM_DELIMITATION_TAG;
      static const tag_type SEQ_DELIMITATION_TAG;
      static const tag_type PIXEL_DATA_TAG;

      static const Tag ITEM;
      static const Tag ITEM_DELIMITATION;
      static const Tag SEQ_DELIMITATION;
      static const Tag PIXEL_DATA;

    private:
      group_type m_group;
      element_type m_element;

    public:
      constexpr Tag()
        : m_group(0),
          m_element(0)
      {}

      explicit constexpr Tag(tag_type tag)
        : m_group(tag >> 16),
          m_element(0xFFFF & tag)
      {}

      explicit constexpr Tag(group_type group, element_type element)
        : m_group(group),
          m_element(element)
      {}

      const group_type& group() const;
      group_type& group();
      const element_type& element() const;
      element_type& element();

      tag_type tag() const;

      std::string str() const;

      bool file_meta() const;

      bool operator==(const Tag& other) const;
      bool operator!=(const Tag& other) const;
      bool operator<(const Tag& other) const;

      bool is_item_tag() const;
      bool is_item_delimitation_tag() const;
      bool is_sequence_delimitation_tag() const;
      bool is_private() const;
      bool is_private_owner() const;

      friend std::ostream& operator<<(std::ostream& os, const Tag& tag);
  };
}
