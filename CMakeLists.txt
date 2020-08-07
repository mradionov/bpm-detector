cmake_minimum_required(VERSION 3.10)

project(BpmDetector VERSION 0.0)

set(CMAKE_BUILD_TYPE Debug)

add_executable(bpm
  src/main.cpp
  src/wave_parse.cpp
  src/wave_plot.cpp
)

target_compile_features(bpm PUBLIC cxx_std_14)
target_compile_options(bpm PRIVATE -Werror -Wall -Wextra)
set_target_properties(bpm PROPERTIES CXX_EXTENSIONS OFF)
