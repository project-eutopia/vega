#pragma once

#include <memory>
#include "vega/dicom/data_element.h"

namespace vega {
  namespace dicom {
    /**
     * \brief A useful wrapper class over the more standard DataElement.
     *
     * This is a template class, where the possible types are defined in vega/include/dictionary_data.h.
     * This means that instead of needing to supply specific tags or VR, you can create a new element
     * simply like
     * ```
     * auto element = std::make_shared<vega::dicom::Element<vega::dictionary::PatientName>>();
     * ```
     * and this element will already have the correct Tag, VR, and will have the correct
     * \link vega/manipulator.h manipulator\endlink of vega::PN_Manipulator built in.
     */
    template <typename T>
    class Element {
      private:
        std::shared_ptr<DataElement> m_data_element;

      public:
        /**
         * Builds a new data element with the Tag and VR corresponding to the given type T.
         * Note that it will fail if the type T does not define a unique Tag, such as
         * vega::dictionary::CurveDimensions which has a range of tags `(50xx,0005)`.
         */
        Element();
        /**
         * Builds a new data element with the given \p tag, and VR corresponding to the given type T.
         * Note that this will fail if the given \p tag does not agree with the TagMask of type T.
         */
        explicit Element(const Tag& tag);
        /**
         * Builds a new Element that wraps the given \p data_element.
         * Will raise an error if the given type T's Tag or VR does not agree with the DataElement.
         */
        explicit Element(const std::shared_ptr<DataElement>& data_element);

        /**
         * \return a pointer to the manipulator for this Element.
         */
        std::shared_ptr<typename T::manipulator_type> manipulator();
        /// \cond INTERNAL
        std::shared_ptr<DataElement> underlying_data_element() const;
        /// \endcond

        const Tag& tag() const;
        Tag& tag();

        const VR& vr() const;
        VR& vr();
    };
  }
}

#include "vega/dicom/element_impl.h"
