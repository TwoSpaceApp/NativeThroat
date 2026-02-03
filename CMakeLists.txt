cmake_minimum_required(VERSION 3.10)
project(NativeThroat)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(oboe)

include(FetchContent)
FetchContent_Declare(
        Ogg
        GIT_REPOSITORY https://github.com/xiph/ogg.git
        GIT_TAG        v1.3.5
)
set(SKIP_INSTALL_ALL ON)
FetchContent_MakeAvailable(Ogg)

set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)

add_library(native_audio SHARED src/native_audio.cpp src/AudioEngine.cpp)

target_include_directories(native_audio PUBLIC
        src
        ${LIBS_DIR}/include
)

target_link_libraries(native_audio
        oboe
        log
        ogg

        ${LIBS_DIR}/${ANDROID_ABI}/libopus.a
        ${LIBS_DIR}/${ANDROID_ABI}/libopusfile.a
)