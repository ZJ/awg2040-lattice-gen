cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_custom_target(
	version
	${CMAKE_COMMAND} -DSRC=${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
	-DDST=${CMAKE_CURRENT_BINARY_DIR}/version.h
	-P ${CMAKE_CURRENT_SOURCE_DIR}/version.cmake
)
add_custom_target(
	sampleFile
	${CMAKE_COMMAND} -DSRC="${CMAKE_CURRENT_SOURCE_DIR}/sampleFile.h.in"
	-DDST="${CMAKE_CURRENT_BINARY_DIR}/sampleFile.h"
	-DSAMPLE="${CMAKE_CURRENT_SOURCE_DIR}/AWGSpec-sample.csv"
	-P "${CMAKE_CURRENT_SOURCE_DIR}/sampleFile.cmake"
)

add_subdirectory(pulseTrain)
add_subdirectory(awg)
add_subdirectory(latticePair)

add_library(options "options.cpp")

add_executable(awg2040-lattice-gen "main.cpp")
target_link_libraries(awg2040-lattice-gen latticePair options)
add_dependencies(awg2040-lattice-gen version sampleFile)

install(TARGETS awg2040-lattice-gen RUNTIME DESTINATION . CONFIGURATIONS RELEASE)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	find_program(MINGW_LIBSTDCXX NAMES libstdc++-6.dll PATHS ${MINGW_GCC_LIB_PATH} NO_CMAKE_FIND_ROOT_PATH NO_DEFAULT_PATH)
	find_program(MINGW_LIBGCC NAMES libgcc_s_sjlj-1.dll PATHS ${MINGW_GCC_LIB_PATH} NO_CMAKE_FIND_ROOT_PATH NO_DEFAULT_PATH)
	find_library(MINGW_LIBWINPTHREAD NAMES libwinpthread-1.dll winpthread pthread)
	install(FILES ${MINGW_LIBSTDCXX} ${MINGW_LIBGCC} ${MINGW_LIBWINPTHREAD} DESTINATION . CONFIGURATIONS RELEASE) 
endif()
