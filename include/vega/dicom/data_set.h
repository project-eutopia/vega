#pragma once

#include <stdint.h>
#include <memory>
#include <map>

#include "vega/vega.h"
#include "vega/tag.h"
#include "vega/formatter.h"
#include "vega/dicom/element.h"

namespace vega {
  namespace dictionary {
    class Page;
    class PrivateOwnerBlocks;
  }

  namespace dicom {
    class DataElement;

    /**
     * \brief Stores a set of \link DataElement DataElements\endlink.
     *
     * This class corresponds to the part of a DICOM file called a "data set"
     * ([see DICOM documentation](http://dicom.nema.org/dicom/2013/output/chtml/part05/chapter_7.html)).
     * Simply put a DataSet is a map from \link Tag Tags\endlink to \link DataElement DataElements\endlink,
     * and so there can be at most one DataElement per Tag.
     */
    class DataSet : public std::enable_shared_from_this<DataSet> {
      public:
        typedef uint32_t length_type;

        length_type length;

      private:
        std::weak_ptr<DataElement> m_parent;
        std::map<Tag, std::shared_ptr<DataElement>> m_elements;
        std::shared_ptr<dictionary::PrivateOwnerBlocks> m_private_owner_blocks;

      public:
        static const length_type UNDEFINED_LENGTH;

        class iterator
        {
          public:
            typedef iterator self_type;
            typedef decltype(m_elements)::iterator iterator_type;

            iterator(iterator_type it);

            operator iterator_type() { return m_it; }
            self_type& operator++();
            std::shared_ptr<DataElement>& operator*();
            bool operator==(const self_type& rhs) const;
            bool operator!=(const self_type& rhs) const;

          private:
            iterator_type m_it;
        };

        class const_iterator
        {
          public:
            typedef const_iterator self_type;
            typedef decltype(m_elements)::const_iterator iterator_type;

            const_iterator(iterator_type it);

            operator iterator_type() { return m_it; }
            self_type& operator++();
            std::shared_ptr<const DataElement> operator*();
            bool operator==(const self_type& rhs) const;
            bool operator!=(const self_type& rhs) const;

          private:
            iterator_type m_it;
        };

        /**
         * Creates a blank DataSet.
         * \param parent is the parent DataElement of this DataSet (`nullptr` when this DataSet is at the root of the DICOM file)
         */
        DataSet(std::shared_ptr<DataElement> parent = nullptr);

        static std::shared_ptr<DataSet> from_json(const std::string& json_string);
        static std::shared_ptr<DataSet> from_json(std::stringstream& json_string, std::shared_ptr<DataElement> parent = nullptr);

        const std::weak_ptr<DataElement>& parent() const;
        std::weak_ptr<DataElement>& parent();

        /// Adds a new DataElement to this DataSet.
        void add_data_element(std::shared_ptr<DataElement> data_element);

        /// \cond false
        /**
         * Returns the dictionary::Page corresponding to the given Tag.
         * The need for a method here is due to the complicated nature of private tags
         * in the DICOM standard.  The details of a given Tag can depend on other tags
         * in the DataSet.  Users will not need to use this method.
         */
        std::shared_ptr<const dictionary::Page> page_for(const Tag& tag) const;
        /**
         * Returns the dictionary::Page corresponding to the given name.
         */
        std::shared_ptr<const dictionary::Page> page_for(const std::string& name) const;
        /// \endcond

        /// Adds a new \link Element Element<T>\endlink to this DataSet.
        template <typename T>
        void add_element(std::shared_ptr<Element<T>> element) {
          if (this->data_element(element->tag())) throw vega::Exception("DataSet::add_element() -- Cannot add new element as it already exists");
          this->m_elements.emplace(element->tag(), element->underlying_data_element());
        }

        bool is_undefined_length() const;

        iterator begin() {
          return iterator(m_elements.begin());
        }
        const_iterator begin() const {
          return const_iterator(m_elements.begin());
        }

        iterator end() {
          return iterator(m_elements.end());
        }
        const_iterator end() const {
          return const_iterator(m_elements.end());
        }

        size_t size() const;
        std::shared_ptr<DataElement> data_element(const std::string& name);
        std::shared_ptr<DataElement> data_element(const Tag& tag);
        std::shared_ptr<DataElement> data_element(const TagMask& tag_mask);
        std::shared_ptr<const DataElement> data_element(const std::string& tag) const;
        std::shared_ptr<const DataElement> data_element(const Tag& tag) const;

        template <typename T>
        std::shared_ptr<Element<T>> new_element() {
          if (this->data_element(T::tag_mask)) throw vega::Exception("DataSet::new_element() -- Cannot create new element as it already exists");

          auto element = std::make_shared<Element<T>>();
          this->add_data_element(element->underlying_data_element());
          return element;
        }

        template <typename T>
        std::shared_ptr<Element<T>> new_element(const Tag& tag) {
          if (this->data_element(T::tag_mask)) throw vega::Exception("DataSet::new_element() -- Cannot create new element as it already exists");

          auto element = std::make_shared<Element<T>>(tag);
          this->add_data_element(element->underlying_data_element());
          return element;
        }

        template <typename T>
        std::shared_ptr<Element<T>> element() {
          auto data_element = this->data_element(T::tag_mask);

          if (data_element) {
            return std::make_shared<Element<T>>(data_element);
          }
          else {
            return nullptr;
          }
        }

        iterator erase(iterator it) {
          return iterator(m_elements.erase(iterator::iterator_type(it)));
        }

        void erase(const Tag& tag);
        void erase(const TagMask& tag_mask);
        void erase(const std::shared_ptr<DataElement>& data_element);

        template <typename T>
        void erase(const std::shared_ptr<Element<T>>& element) { erase(element->tag()); }

        template <typename T>
        void erase() { erase(T::tag_mask); }

        bool operator==(const DataSet& other) const;
        bool operator!=(const DataSet& other) const;

        void log(Formatter& formatter) const;
        void json(Formatter& formatter) const;

      private:
        void add_private_owner_block_if_relevant(std::shared_ptr<DataElement> data_element);
    };
  }
}
