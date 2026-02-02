cmake_minimum_required(VERSION 3.10)
project(NativeThroat)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(oboe)

add_library(native_audio SHARED src/native_audio.cpp src/AudioEngine.cpp)

target_include_directories(native_audio PUBLIC src)

target_link_libraries(native_audio oboe)
