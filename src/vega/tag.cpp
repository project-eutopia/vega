#include "vega/tag.h"

#include <iomanip>
#include <sstream>

namespace vega {
  const Tag::tag_type Tag::ITEM_TAG = 0xFFFEE000;
  const Tag::tag_type Tag::ITEM_DELIMITATION_TAG = 0xFFFEE00D;
  const Tag::tag_type Tag::SEQ_DELIMITATION_TAG = 0xFFFEE0DD;
  const Tag::tag_type Tag::PIXEL_DATA_TAG = 0x7FE00010;

  const Tag Tag::ITEM(ITEM_TAG);
  const Tag Tag::ITEM_DELIMITATION(ITEM_DELIMITATION_TAG);
  const Tag Tag::SEQ_DELIMITATION(SEQ_DELIMITATION_TAG);
  const Tag Tag::PIXEL_DATA(PIXEL_DATA_TAG);

  const Tag::group_type& Tag::group() const { return m_group; }
  Tag::group_type& Tag::group() { return m_group; }

  const Tag::element_type& Tag::element() const { return m_element; }
  Tag::element_type& Tag::element() { return m_element; }

  Tag::tag_type Tag::tag() const { return (m_group << 16) | m_element; }

  std::string Tag::str() const {
    std::stringstream ss;
    ss << (*this);
    return ss.str();
  }

  bool Tag::is_file_meta() const {
    return m_group == 0x2;
  }

  bool Tag::operator==(const Tag& other) const {
    return (this->group() == other.group()) && (this->element() == other.element());
  }

  bool Tag::operator!=(const Tag& other) const {
    return !(*this == other);
  }

  bool Tag::operator<(const Tag& other) const {
    if (this->group() < other.group()) {
      return true;
    }
    else if (this->group() > other.group()) {
      return false;
    }
    else {
      return this->element() < other.element();
    }
  }

  bool Tag::is_item_tag() const { return this->tag() == ITEM_TAG; }
  bool Tag::is_item_delimitation_tag() const { return this->tag() == ITEM_DELIMITATION_TAG; }
  bool Tag::is_sequence_delimitation_tag() const { return this->tag() == SEQ_DELIMITATION_TAG; }
  bool Tag::is_private() const { return (this->group() & 1) == 1; }
  bool Tag::is_private_owner() const { return this->is_private() && (this->element() & 0xFF00) == 0; }

  std::ostream& operator<<(std::ostream& os, const Tag& tag) {
    std::ios::fmtflags f( os.flags() );
    os << "(";
    os << std::hex << std::uppercase;
    os << std::setfill('0') << std::setw(4) << tag.group();
    os << ",";
    os << std::setfill('0') << std::setw(4) << tag.element();
    os << ")";
    os.flags(f);
    return os;
  }

  std::istream& operator>>(std::istream& is, Tag& tag) {
    std::ios::fmtflags f( is.flags() );
    char c;
    is >> c; // '('
    is >> std::hex >> std::uppercase;
    is >> std::setfill('0') >> std::setw(4) >> tag.group();
    is >> c; // ','
    is >> std::setfill('0') >> std::setw(4) >> tag.element();
    is >> c; // ')'
    is.flags(f);
    return is;
  }
}
