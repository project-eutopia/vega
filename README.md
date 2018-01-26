# vega

C++ library for reading/writing/editing DICOM files.
Detailed documentation can be found [here](https://project-eutopia.github.io/vega/).

## Getting started

To get started, clone the repository:

```bash
git clone https://github.com/project-eutopia/vega.git
```

To compile and install the shared library to your system, take the following steps:

```bash
mkdir build
cd build
cmake ..
sudo make -j8 install
```

To compile code that uses this library, you should link the library, the zlib library, and have to use at least C++11 standard:

```bash
clang++ --std=c++11 -lvega -lz my_code.cpp -o my_code
```

## Reading/writing files

Here is a simple example of using the library to read a file in and print a string representation of the content to standard out.

```C++
#include <string>

#include "vega/dictionary/dictionary.h"
#include "vega/dicom/file.h"

int main() {
  // Set the dictionary file
  vega::dictionary::Dictionary::set_dictionary("/path/to/dictionary/dictionary.txt");
  
  // Read the DICOM file in
  const std::string file_name = "/path/to/dicom/file/dicom.dcm";
  vega::dicom::File file(file_name);
  
  // Print a human-friendly representation of the file to std::cout
  vega::Formatter formatter(std::cout);
  file.data_set()->log(formatter);
}
```

To write a DICOM file, you call the `vega::dicom::File::write` method, either passing in a string that represents the file name, or a `std::shared_ptr<std::ostream>` pointer to an output stream.

## Creating new DICOM file

To create a new DICOM file, use the `File` constructor with either `const SOPClass&` reference, or a `const SOPClass&` reference and `const UID&` reference to a SOP instance UID.

```c++
vega::dicom::File file(vega::dicom::SOPClass("RT Plan Storage"), vega::UID("1.2.3.4.5"));
```

## Editing a DICOM file

The rough [structure of a DICOM file](http://dicom.nema.org/dicom/2013/output/chtml/part05/chapter_7.html) is that at the root of a DICOM file is a single [**data set**](https://project-eutopia.github.io/vega/classvega_1_1dicom_1_1DataSet.html), where each data set contains a set of [**data elements**](https://project-eutopia.github.io/vega/classvega_1_1dicom_1_1DataElement.html).
A data element is identified by a unique [**tag**](https://project-eutopia.github.io/vega/classvega_1_1Tag.html), which is a pair of 2 byte unsigned integers, often represented in hexadecimal, for instance (300A,00C2) is the tag corresponding to "Beam Name".
A data set contains at most one data element for each tag, so it is effectively a map from tags to data elements.
Each data element contains some data whose interpretation is determined by the tag, for instance (0010,0010) which corresponds to the patient's name.
The storage format of the data is determined by the [**value representation**](https://project-eutopia.github.io/vega/classvega_1_1VR.html) (or VR).

Each `File` object has a `data_set()` method which returns a `shared_ptr` to the root `vega::dicom::DataSet` object.
`DataSet` has iterators for going through each `DataElement`, and also offers many useful methods for selecting a specific `DataElement`.
You can search for a `DataElement` using a dictionary element string name, e.g. `"PatientName"` or a tag like `Tag{0x0010,0x0010}`.

The data stored inside each `DataElement` is accessed through what is called a [**manipulator**](https://project-eutopia.github.io/vega/namespacevega_1_1manipulators.html).
Each manipulator, for instance `vega::manipulators::DateManipulator` is used for data elements with VR DA (corresponding to "date").
This manipulator acts very much like a `std::vector` of `vega::Date` objects which makes reading, adding, and modifying data very easy.
The translation to the behind-the-scenes DICOM formats is hidden from the user.

In addition to `DataElement`, there is also an easier to use class `vega::dicom::Element<T>`, where `T` is a dictionary element type (e.g. `vega::dictionary::PatientName`, see `include/vega/dictionary_data.h` for all possibilities).
The benefit of using this class is that you do not need to make explicit the manipulator class, as it is implied by the type (e.g. `vega::dictionary::PatientName::manipulator_type` is the expected `PersonNameManipulator`).
As an example of adding patient name to a new data set:

```c++
auto element = data_set->new_element<vega::dictionary::PatientName>();
element->manipulator()->push_back("Smith^Alice");
```
