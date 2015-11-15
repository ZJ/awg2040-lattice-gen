cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(
	version
	${CMAKE_COMMAND} -DSRC=${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
	-DDST=${CMAKE_CURRENT_BINARY_DIR}/version.h
	-P ${CMAKE_CURRENT_SOURCE_DIR}/version.cmake
)

add_library(options "options.cpp")
add_library(pulseTrain "pulseTrain.cpp")
add_library(awg "awg.cpp")
target_link_libraries(awg pulseTrain)
add_library(latticePair "latticePair.cpp")
target_link_libraries(latticePair awg pulseTrain)

add_executable(awg2040-lattice-gen "main.cpp")
target_link_libraries(awg2040-lattice-gen latticePair options)
add_dependencies(awg2040-lattice-gen version)
install(TARGETS awg2040-lattice-gen RUNTIME DESTINATION .)
