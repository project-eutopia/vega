#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <string>

#include "vega/vega.h"
#include "vega/dicom/data_element_header.h"
#include "vega/dictionary/dictionary.h"
#include "vega/logger.h"

#include "vega/manipulators/value_manipulator.h"

namespace vega {
  namespace dicom {
    class DataSet;

    class DataElement {
      private:
        DataElementHeader m_header;
        std::shared_ptr<const dictionary::Page> m_page;

        std::weak_ptr<DataSet> m_parent;
        std::vector<std::shared_ptr<DataSet>> m_data_sets;

        std::shared_ptr<manipulators::ValueManipulator> m_manipulator;

      public:
        DataElement(std::shared_ptr<DataSet> parent = nullptr);
        // Can only use this constructor if the corresponding dictionary page with the
        // given name has a unique Tag
        DataElement(const std::string& name, std::shared_ptr<DataSet> parent = nullptr);
        DataElement(const Tag& tag, std::shared_ptr<DataSet> parent = nullptr);
        DataElement(const Tag& tag, const VR& vr, std::shared_ptr<DataSet> parent = nullptr);

        const std::shared_ptr<const dictionary::Page>& page() const;
        void set_page(std::shared_ptr<const dictionary::Page> page);

        const DataElementHeader& header() const;
        DataElementHeader& header();

        const Tag& tag() const;
        Tag& tag();

        const VR& vr() const;
        VR& vr();

        const DataElementHeader::length_type& length() const;
        DataElementHeader::length_type& length();

        const std::weak_ptr<DataSet>& parent() const;
        std::weak_ptr<DataSet>& parent();

        const std::vector<std::shared_ptr<DataSet>>& data_sets() const;
        std::vector<std::shared_ptr<DataSet>>& data_sets();

        std::shared_ptr<manipulators::ValueManipulator> manipulator() { return m_manipulator; }
        std::shared_ptr<const manipulators::ValueManipulator> manipulator() const {
          return std::static_pointer_cast<const manipulators::ValueManipulator>(m_manipulator);
        }

        template <typename T>
        void set_manipulator(std::shared_ptr<T> manipulator) {
          if (!manipulator->is_valid_for(this->vr())) {
            throw vega::Exception(std::string("DataElement::set_manipulator, received manipulator does not support VR ") + this->vr().str());
          }
          m_manipulator = std::dynamic_pointer_cast<manipulators::ValueManipulator>(manipulator);
        }

        template <typename T>
        std::shared_ptr<T> get_manipulator() {
          this->vr().validate_value_manipulator<T>();

          // Brand new
          if (!m_manipulator) {
            m_manipulator = std::make_shared<T>();
            return std::static_pointer_cast<T>(m_manipulator);
          }

          // If type is same as already stored manipulator, just return
          std::shared_ptr<T> manipulator = std::dynamic_pointer_cast<T>(m_manipulator);
          if (manipulator) return manipulator;

          // Otherwise, update internal manipulator and return
          m_manipulator = std::make_shared<T>(m_manipulator->raw_value());
          return std::static_pointer_cast<T>(m_manipulator);
        }

        // Only can use for VR: UI
        std::string str() const;

        bool is_sequence() const;
        bool is_undefined_length() const;

        std::vector<std::shared_ptr<DataSet>>::iterator begin() {
          return m_data_sets.begin();
        }
        std::vector<std::shared_ptr<DataSet>>::const_iterator begin() const {
          return m_data_sets.begin();
        }

        std::vector<std::shared_ptr<DataSet>>::iterator end() {
          return m_data_sets.end();
        }
        std::vector<std::shared_ptr<DataSet>>::const_iterator end() const {
          return m_data_sets.end();
        }

        bool operator==(const DataElement& other) const;
        bool operator!=(const DataElement& other) const;

        void log(Logger& logger) const;
    };
  }
}
