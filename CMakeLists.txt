cmake_minimum_required(VERSION 3.10)

project(BpmDetector VERSION 0.0)

add_executable(bpm src/main.cpp)
target_compile_features(bpm PUBLIC cxx_std_14)
set_target_properties(bpm PROPERTIES CXX_EXTENSIONS OFF)
