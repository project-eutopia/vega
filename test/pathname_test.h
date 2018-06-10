#pragma once

#include "test_helper.h"

#include "vega/pathname.h"

using namespace vega;

TEST(PathnameTest, pathname_with_folder_and_extension_test) {
  Pathname pathname("/home/chris/README.md");

  EXPECT_EQ(pathname.full_name(), "/home/chris/README.md");
  EXPECT_EQ(pathname.extension(), "md");
  EXPECT_EQ(pathname.base_name(), "README");
  EXPECT_EQ(pathname.folder(), "/home/chris/");
}

TEST(PathnameTest, pathname_with_just_slash) {
  Pathname pathname("/");

  EXPECT_EQ(pathname.full_name(), "/");
  EXPECT_EQ(pathname.extension(), "");
  EXPECT_EQ(pathname.base_name(), "");
  EXPECT_EQ(pathname.folder(), "/");
}

TEST(PathnameTest, pathname_with_dot_in_folder_test) {
  Pathname pathname("/my/folder.d/what is this");

  EXPECT_EQ(pathname.full_name(), "/my/folder.d/what is this");
  EXPECT_EQ(pathname.extension(), "");
  EXPECT_EQ(pathname.base_name(), "what is this");
  EXPECT_EQ(pathname.folder(), "/my/folder.d/");
}

TEST(PathnameTest, pathname_no_folder_or_extension_test) {
  Pathname pathname("a_file_name");

  EXPECT_EQ(pathname.full_name(), "a_file_name");
  EXPECT_EQ(pathname.extension(), "");
  EXPECT_EQ(pathname.base_name(), "a_file_name");
  EXPECT_EQ(pathname.folder(), "./");
}

TEST(PathnameTest, pathname_no_folder_with_extension_test) {
  Pathname pathname("source.cpp");

  EXPECT_EQ(pathname.full_name(), "source.cpp");
  EXPECT_EQ(pathname.extension(), "cpp");
  EXPECT_EQ(pathname.base_name(), "source");
  EXPECT_EQ(pathname.folder(), "./");
}

TEST(PathnameTest, pathname_with_folder_and_dot_at_start_of_name) {
  Pathname pathname("/project/.gitignore");

  EXPECT_EQ(pathname.full_name(), "/project/.gitignore");
  EXPECT_EQ(pathname.extension(), "");
  EXPECT_EQ(pathname.base_name(), ".gitignore");
  EXPECT_EQ(pathname.folder(), "/project/");
}

TEST(PathnameTest, pathname_no_folder_and_dot_at_start_of_name) {
  Pathname pathname(".gitignore");

  EXPECT_EQ(pathname.full_name(), ".gitignore");
  EXPECT_EQ(pathname.extension(), "");
  EXPECT_EQ(pathname.base_name(), ".gitignore");
  EXPECT_EQ(pathname.folder(), "./");
}

TEST(PathnameTest, pathname_istream_test) {
  Pathname pathname;
  std::string s("/hello/world.txt");
  std::stringstream ss(s);

  ss >> pathname;

  EXPECT_EQ(pathname.full_name(), "/hello/world.txt");
  EXPECT_EQ(pathname.extension(), "txt");
  EXPECT_EQ(pathname.base_name(), "world");
  EXPECT_EQ(pathname.folder(), "/hello/");
}

TEST(PathnameTest, pathname_assignment_from_string_test) {
  Pathname pathname;
  pathname = "/bin/bash";

  EXPECT_EQ(pathname.full_name(), "/bin/bash");
  EXPECT_EQ(pathname.extension(), "");
  EXPECT_EQ(pathname.base_name(), "bash");
  EXPECT_EQ(pathname.folder(), "/bin/");

  pathname = "/usr/bin/env";

  EXPECT_EQ(pathname.full_name(), "/usr/bin/env");
  EXPECT_EQ(pathname.extension(), "");
  EXPECT_EQ(pathname.base_name(), "env");
  EXPECT_EQ(pathname.folder(), "/usr/bin/");
}

TEST(PathnameTest, pathname_implicit_string_cast) {
  Pathname pathname("/foo/bar.baz");
  std::string s = pathname;
  EXPECT_EQ(s, "/foo/bar.baz");
}

TEST(PathnameTest, pathname_plus_operator) {
  Pathname pathname = Pathname("/") + "home" + Pathname("/user/") + std::string("file.txt");

  EXPECT_EQ(pathname.full_name(), "/home/user/file.txt");
  EXPECT_EQ(pathname.extension(), "txt");
  EXPECT_EQ(pathname.base_name(), "file");
  EXPECT_EQ(pathname.folder(), "/home/user/");
}
