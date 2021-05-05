cmake_minimum_required(VERSION 3.15)

project(Cafe.Environment CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/CafeCommon/cmake/CafeCommon.cmake)

set(SOURCE_FILES
    src/Cafe/Misc/Environment.cpp)

set(HEADERS
    src/Cafe/Misc/Environment.h)

add_library(Cafe.Environment ${SOURCE_FILES} ${HEADERS})

target_compile_features(Cafe.Environment PUBLIC cxx_std_20)

target_include_directories(Cafe.Environment PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(Cafe.Environment
    PUBLIC CONAN_PKG::Cafe.Encoding CONAN_PKG::Cafe.TextUtils)

install(TARGETS Cafe.Environment
    EXPORT Environment)

install(DIRECTORY src/
    TYPE INCLUDE
    FILES_MATCHING PATTERN "*.h")

install(EXPORT Environment
    DESTINATION cmake/Environment)
