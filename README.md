[![Build Status](https://travis-ci.org/project-eutopia/vega.svg?branch=master)](https://travis-ci.org/project-eutopia/vega)

# vega

C++ library for reading/writing/editing DICOM files.
Detailed documentation can be found [here](https://project-eutopia.github.io/vega/).
Note that while this library is fully functional, it is still in an early alpha stage of development so changes to the library can be expected.

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

## Installing `vega` binary

This library comes packaged with a runtime executable `vega` that can is installed by `make install` to `/usr/local/bin/vega`.
It reads from the dictionary file installed to `/usr/local/share/vega/dictionary.txt`, and can be used to process DICOM files in the terminal.
A DICOM file can either be piped in as STDIN from a DICOM file,
```
cat my_dicom_file.dcm | vega
```
or it can be specified in an input flag
```
vega --input=my_dicom_file.dcm
```
The input can also be a comma separated list of filenames, with the condition that there must be and equal number of filenames separated by commas in the `output` field,
```
vega --input=file1.dcm,file2.dcm --output=file1b.dcm,file2b.dcm
```
If the `suffix` option is passed in, then instead that will be appended to the filename for each output file.
For instance, the following will anonymize each input file, and write them to `file1.anon.dcm` and `file2.anon.dcm`,
```
vega --input=file1.dcm,file2.dcm --suffix=anon --anonymize
```
Lastly, input files can be piped in to the executable as follows:
```
ls *.dcm | vega --anonymize --suffix=anon --folder=/my/data/directory
```
Here the `--folder` option is being used to specify the output directory to write the files to.


For output, `vega` supports 3 file formats: `".dcm"` for DICOM format, `".json"` for JSON file format, and `".txt"` for human-readable plain text files.
If `--output` is just equal to the extension (e.g. `--output=json`), then the DICOM file will be printed to STDOUT in that format.
If `--output` is a full filename with the relevant extension, then it will be wrote to that file.

The `vega` binary also supports options to modify the DICOM file before outputting.
The supported file options are:
- `--remove_undefined_lengths`: forces the output file(s) to have no data elements with udnefined lengths
- `--anonymize` or `--anonymize=my_new_patient_id`: anonymizes the input file(s)

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
