cmake_minimum_required(VERSION 3.10)

project(BpmDetector VERSION 0.0)

set(CMAKE_BUILD_TYPE Debug)

set(BPM_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(BPM_TEST_DIR ${PROJECT_SOURCE_DIR}/test)
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/catch)

set(BPM_SOURCE_FILES
  ${BPM_SOURCE_DIR}/main.cpp
  ${BPM_SOURCE_DIR}/ppm_image.cpp
  ${BPM_SOURCE_DIR}/wave_parse.cpp
  ${BPM_SOURCE_DIR}/wave_plot.cpp
)

set(BPM_TEST_FILES
  ${BPM_TEST_DIR}/main.cpp
  ${BPM_TEST_DIR}/dummy.cpp
)

add_executable(bpm ${BPM_SOURCE_FILES})

target_compile_features(bpm PUBLIC cxx_std_14)
target_compile_options(bpm PRIVATE -Werror -Wall -Wextra)
set_target_properties(bpm PROPERTIES CXX_EXTENSIONS OFF)

option(BUILD_TESTS "Determines whether to build tests" OFF)
if (BUILD_TESTS)
  enable_testing()

  add_library(Catch INTERFACE)
  target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

  add_executable(unit_tests ${BPM_TEST_FILES})
  target_link_libraries(unit_tests PUBLIC Catch)
endif()
