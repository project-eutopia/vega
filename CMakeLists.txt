cmake_minimum_required (VERSION 3.2.2)
project (vega CXX)

if (NOT CMAKE_BUILD_TYPE MATCHES DEBUG)
  add_definitions(-DNDEBUG)
endif()

include_directories(include)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Ofast -pthread")

# Source codes (except main.cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_SOURCE_DIR}/src/main.cpp")

# Create shared and and development versions of library
add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_library(${PROJECT_NAME}_dev SHARED ${SOURCES})

# Install library and headers
install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)

file(GLOB_RECURSE HEADERS include/${PROJECT_NAME}/*.h)
install (DIRECTORY include/${PROJECT_NAME} DESTINATION include)

# Google test library
# Setup on Ubuntu like (see: https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/)
#
# sudo apt-get install libgtest-dev
# sudo apt-get install cmake # install cmake
# cd /usr/src/gtest
# sudo cmake CMakeLists.txt
# sudo make
# sudo cp *.a /usr/lib
find_package(GTest REQUIRED)

add_executable(main "src/main.cpp")
target_link_libraries(main ${PROJECT_NAME}_dev z)

# Installing `vega` binary to `/usr/local/bin/vega`
add_executable(vega-bin "src/main.cpp")
SET_TARGET_PROPERTIES(vega-bin PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(vega-bin ${PROJECT_NAME} z)
install(TARGETS vega-bin RUNTIME DESTINATION bin)

# Installing vega dictionary file to `/usr/local/share/vega/dictionary.txt`
install(FILES "test/dictionary.txt" DESTINATION share/vega)

# Test command
if (GTest_FOUND)
  include_directories(${GTest_INCLUDE_DIRS})
  add_executable(tests "test/tests.cpp")
  target_link_libraries(tests ${PROJECT_NAME}_dev ${GTEST_LIBRARIES} pthread z)
endif()
