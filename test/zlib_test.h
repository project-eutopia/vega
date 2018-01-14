#pragma once

#include "test_helper.h"

#include "vega/zlib.h"
#include "vega/manipulator.h"
#include "vega/dicom/file.h"
#include <iostream>
#include <fstream>

using namespace vega;

TEST(ZlibTest, inflate_test) {
  std::string filename = tests::path_to_file("data/pydicom/image_dfl.deflate");
  std::ifstream ifs{filename, std::ifstream::binary};
  std::string content{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};

  auto ss = RFC1951::inflate(content);
  EXPECT_EQ(ss->str().size(), 262682);

  testing::internal::CaptureStderr();
  dicom::File file(ss);
  testing::internal::GetCapturedStderr();

  auto sop_instance_uid = file.data_set()->data_element("SOPInstanceUID")->get_manipulator<UI_Manipulator>()->uid();
  EXPECT_EQ(sop_instance_uid.str(), "1.3.6.1.4.1.5962.1.1.0.0.0.977067309.6001.0");
}

TEST(ZlibTest, complete_test) {
  std::string original = "The quick brown fox jumped over the lazy dog.";
  std::string compressed = RFC1951::deflate(original)->str();
  std::string decompressed = RFC1951::inflate(compressed)->str();
  EXPECT_EQ(original, decompressed);
}
