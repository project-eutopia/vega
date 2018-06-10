#pragma once

#include <stdint.h>
#include <memory>
#include <vector>
#include <string>
#include <mutex>

#include "vega/vega.h"
#include "vega/dicom/data_element_header.h"
#include "vega/dictionary/dictionary.h"
#include "vega/formatter.h"

#include "vega/manipulators/value_manipulator.h"

namespace vega {
  namespace dicom {
    class DataSet;
    class Reader;

    /**
     * \class DataElement
     * \brief This class is used to contain the information in a single DataElement.
     *
     * This class corresponds to the part of a DICOM file called a "data element" ([see DICOM documentation](http://dicom.nema.org/dicom/2013/output/chtml/part05/chapter_7.html)).
     *
     * A DataElement has two main parts: a DataElementHeader and the data itself.
     * The DataElementHeader is used to store the Tag which defines what type of data is stored, VR
     * which defines the representation that the data is in, and a \link length_type length\endlink which
     * determines how much data is stored in the DataElement.
     * The length never needs to be touched by the user of this library, because it is always
     * dynamically determined when writing to file anyway.
     * Furthermore, the user should also never need to concern themselves with modifying the tag
     * or vr directly.  This is because they are automatically filled when reading from a DICOM
     * file, and they are automatically set for you when you instantiate a new DataElement yourself.
     *
     * The data stored in the DataElement can take one of two forms:  raw data, or sequence data.
     * The details are explained in
     * [the DICOM documentation](http://dicom.nema.org/dicom/2013/output/chtml/part05/chapter_7.html),
     * but it is not necessary to understand these details to use this library.
     * The main thing to know is that when the VR of a DataElement is vr::SQ (is_sequence() returns `true`),
     * the DataElement stores a vector of \link DataSet DataSets\endlink,
     * and otherwise (is_sequence() returns `false`) it stores raw data
     * that is accessible through a \link vega/manipulator.h manipulator\endlink.
     *
     * ### Sequence
     *
     * When the DataElement is a sequence, you can access the DataSet vector through the data_sets()
     * method.  This class also provides iterators through the begin() and end() methods so
     * the \link DataSet DataSets\endlink can be looped over easily.
     * ```
     * for (const auto& data_set : data_element) {
     *   // Do something
     * }
     * ```
     *
     * ### Non-sequence
     *
     * In this case, the DataElement stores raw data accessible through a
     * \link vega/manipulator.h manipulator\endlink object.
     * The difficulty with using manipulators comes from the fact that what data is stored and how
     * it is stored varies depending on the VR.  This means that this information cannot be known
     * at compile-time, and so must be provided through the templates of get_manipulator() and
     * set_manipulator().  The advantage of using these manipulators is that they are each designed
     * to make dealing with the data for each type of VR.  See vega/manipulator.h for a list of
     * allowed manipulators and how to use them.
     *
     * Note that it is usually easier to use the wrapper class \link Element Element<T>\endlink
     * instead of DataElement when dealing with raw data.
     */
    class DataElement : public std::enable_shared_from_this<DataElement> {
      private:
        DataElementHeader m_header;
        std::shared_ptr<const dictionary::Page> m_page;

        std::weak_ptr<DataSet> m_parent;
        mutable std::vector<std::shared_ptr<DataSet>> m_data_sets;

        // Points to reader if content not yet read, or nullptr if already read
        mutable std::shared_ptr<Reader> m_reader;
        std::streampos m_start;

        mutable std::shared_ptr<manipulators::ValueManipulator> m_manipulator;

        // For making lazy loading thread-safe
        mutable std::mutex m_mutex;

      public:
        /// Creates a blank DataElement with the given \p parent.
        /// \param parent is the parent DataSet of the DataElement
        DataElement(std::shared_ptr<DataSet> parent = nullptr);
        /**
         * Creates a blank DataElement with tag given by the \p name.
         *
         * The Tag and VR is read from the Dictionary using the \p name parameter.
         * Can only use this constructor if the corresponding dictionary::Page has
         * a single unique Tag.  For example the dictionary entry `"CurveDimensions"` is
         * defined to have tag `(50xx,0005)` where `xx` can be any hexadecimal digits,
         * and so this would not be permitted.
         *
         * \param name is the name of a Dictionary entry to read VR and Tag data from (e.g. `"PatientName"`).
         * \param parent is the parent DataSet of the DataElement
         */
        DataElement(const std::string& name, std::shared_ptr<DataSet> parent = nullptr, bool implicit_vr = false);
        /**
         * Creates a blank DataElement with tag given by the \p tag.
         *
         * Will raise an error if the given Tag is not present in the Dictionary,
         * or if the Tag has a variable VR.  For instance pixel data with tag `(0x7FE0,0x0010)`
         * can have VR of vr::OB or vr::OW, and so you should use the constructor which takes
         * a VR also.
         *
         * \param tag is the Tag of this DataElement.
         * \param parent is the parent DataSet of the DataElement
         */
        DataElement(const Tag& tag, std::shared_ptr<DataSet> parent = nullptr, bool implicit_vr = false);
        /**
         * Creates a blank DataElement with tag given by the \p tag and \p VR.
         *
         * Will raise an error if the given dictionary::Page corresponding to the Tag is not
         * found or if the VR is incompatible with it.
         *
         * \param tag is the Tag of this DataElement.
         * \param vr is the VR of this DataElement.
         * \param parent is the parent DataSet of the DataElement (`nullptr` when at the root of the DICOM file)
         */
        DataElement(const Tag& tag, const VR& vr, std::shared_ptr<DataSet> parent = nullptr);

        void set_value_field(const std::shared_ptr<Reader>& reader, const std::streampos& start);

        static std::shared_ptr<DataElement> from_json(std::stringstream& json_string, const Tag& tag, std::shared_ptr<DataSet> parent = nullptr);

        /// \return the dictionary::Page corresponding to this DataElement's Tag.
        const std::shared_ptr<const dictionary::Page>& page() const;
        /// Sets the dictionary::Page for blank \link DataElement DataElements\endlink.
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

        /// \return a vector of the this DataElement's sequence of DataSet objects.
        const std::vector<std::shared_ptr<DataSet>>& data_sets() const;
        /// This method is used when building a new DataElement sequence by giving the user a reference.
        /// \return a reference to the vector of the this DataElement's sequence of DataSet objects.
        std::vector<std::shared_ptr<DataSet>>& data_sets();

        /// \cond INTERNAL
        std::shared_ptr<manipulators::ValueManipulator> manipulator() { lazy_load(); return m_manipulator; }
        std::shared_ptr<const manipulators::ValueManipulator> manipulator() const {
          lazy_load();
          return std::static_pointer_cast<const manipulators::ValueManipulator>(m_manipulator);
        }
        /// \endcond

        /**
         * Given a user filled \link vega::manipulators manipulator\endlink, this DataElement's
         * internal manipulator will be set to this, and so any stored data is overwritten.
         */
        template <typename T>
        void set_manipulator(std::shared_ptr<T> manipulator) {
          // Nothing to lazy load if setting manipulator directly
          if (m_reader) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_reader = nullptr;
          }

          this->validate_manipulator(*manipulator);
          m_manipulator = std::dynamic_pointer_cast<manipulators::ValueManipulator>(manipulator);
        }

        /**
         * Returns a \link vega/manipulator.h\endlink manipulator object for the user to manipulate
         * the content of this DataElement.  If there is not yet a manipulator object, a new one is
         * created and returned.  If the received type T is compatible with the current existing
         * manipulator, then a pointer to that is cast and returned.  Otherwise if a new type is
         * specified then a new manipulator is created from the existing raw data, so if it currently
         * stores a list of 16 bit integers and you request a manipulator of 32 bit integers, then
         * each 32 bit integer will be a pair of 16 bit integers.
         *
         * ```
         * auto manipulator = data_element->get_manipulator<vega::FL_Manipulator>();
         * manipulator->push_back(3.14f);
         * ```
         */
        template <typename T>
        std::shared_ptr<T> get_manipulator() {
          this->vr().validate_value_manipulator<T>();
          lazy_load();

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

        /// Builds a string representation of the content of this DataElement.
        std::string str() const;

        /// \return `true` when this DataElement is a sequence (has VR of vr::SQ).
        bool is_sequence() const;
        /// \cond
        bool is_undefined_length() const;
        /// \endcond

        std::vector<std::shared_ptr<DataSet>>::iterator begin() {
          lazy_load();
          return m_data_sets.begin();
        }
        std::vector<std::shared_ptr<DataSet>>::const_iterator begin() const {
          lazy_load();
          return m_data_sets.begin();
        }

        std::vector<std::shared_ptr<DataSet>>::iterator end() {
          lazy_load();
          return m_data_sets.end();
        }
        std::vector<std::shared_ptr<DataSet>>::const_iterator end() const {
          lazy_load();
          return m_data_sets.end();
        }

        bool operator==(const DataElement& other) const;
        bool operator!=(const DataElement& other) const;

        void log(Formatter& formatter) const;
        void json(Formatter& formatter) const;

      private:
        template <typename T>
        void validate_manipulator(const T& manipulator) const {
          if (!manipulator.is_valid_for(this->vr())) {
            throw vega::Exception(std::string("DataElement::set_manipulator, received manipulator does not support VR ") + this->vr().str());
          }
        }

        void lazy_load() const;
        void read_finite_sequence(const std::shared_ptr<Reader>& reader) const;
        void read_value_field(const std::shared_ptr<Reader>& reader) const;
    };
  }
}
