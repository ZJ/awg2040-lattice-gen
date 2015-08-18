cmake_minimum_required(VERSION 2.6)
project(awg2040-lattice-gen)
set(awgLS_VERSION_MAJOR 0)
set(awgLS_VERSION_MINOR 1)

configure_file (
"${PROJECT_SOURCE_DIR}/Config.h.in"
"${PROJECT_BINARY_DIR}/Config.h"
)

MESSAGE( STATUS "Value of TRAVIS: $ENV{TRAVIS}")
if(DEFINED ENV{TRAVIS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

include_directories("${PROJECT_BINARY_DIR}")

find_library( GTEST gtest "${PROJECT_SOURCE_DIR}/gtest/builds/native/lib")
set(GTEST_LIBS "${GTEST}" pthread)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
