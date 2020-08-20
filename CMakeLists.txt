cmake_minimum_required(VERSION 3.10)

project(BpmDetector VERSION 0.0 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

set(BPM_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(BPM_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/catch)

set(BPM_SOURCE_FILES
  ${BPM_SOURCE_DIR}/bytestream_reader.cpp
  ${BPM_SOURCE_DIR}/ppm_image.cpp
  ${BPM_SOURCE_DIR}/wave_parse.cpp
  ${BPM_SOURCE_DIR}/wave_plot.cpp
)

set(BPM_TEST_FILES
  ${BPM_SOURCE_DIR}/bytestream_reader_unittest.cpp
  ${BPM_SOURCE_DIR}/wave_parse_unittest.cpp
)

# Library, will be used for executable and for unit tests

add_library(bpm_lib ${BPM_SOURCE_FILES})
target_compile_features(bpm_lib PUBLIC cxx_std_14)
target_compile_options(bpm_lib PRIVATE -Werror -Wall -Wextra)
set_target_properties(bpm_lib PROPERTIES CXX_EXTENSIONS OFF)

# Executable

add_executable(bpm ${BPM_SOURCE_DIR}/main.cpp)
target_link_libraries(bpm PRIVATE bpm_lib)
target_compile_features(bpm_lib PUBLIC cxx_std_14)
target_compile_options(bpm_lib PRIVATE -Werror -Wall -Wextra)
set_target_properties(bpm_lib PROPERTIES CXX_EXTENSIONS OFF)

# Unit tests

option(BUILD_TESTS "Determines whether to build tests" OFF)
if (BUILD_TESTS)
  enable_testing()

  add_library(Catch INTERFACE)
  target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

  add_executable(unit_tests
    ${BPM_TEST_DIR}/catch_main.cpp
    ${BPM_TEST_FILES}
  )
  target_link_libraries(unit_tests
    Catch
    bpm_lib
  )
  target_compile_features(unit_tests PUBLIC cxx_std_14)
  target_compile_options(unit_tests PRIVATE -Werror -Wall -Wextra)
  set_target_properties(unit_tests PROPERTIES CXX_EXTENSIONS OFF)
endif()
