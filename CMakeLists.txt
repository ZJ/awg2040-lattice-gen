cmake_minimum_required(VERSION 2.6)
project(awg2040-lattice-gen)
set(awgLS_VERSION_MAJOR 0)
set(awgLS_VERSION_MINOR 1)

configure_file (
"${PROJECT_SOURCE_DIR}/Config.h.in"
"${PROJECT_BINARY_DIR}/Config.h"
)

include_directories("${PROJECT_BINARY_DIR}")

add_executable(Test src/driveOpts.cpp)
