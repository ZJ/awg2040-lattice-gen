find_package(Git)
if(GIT_FOUND)
	EXECUTE_PROCESS(
		COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=10 --always --dirty
		OUTPUT_VARIABLE GIT_COMMIT
		OUTPUT_STRIP_TRAILING_WHITESPACE
		RESULT_VARIABLE VER_SUCCESS
	)
	if(NOT VER_SUCCESS EQUAL 0)
		SET(GIT_COMMIT "Non-git-tracked source")
	endif()
else()
	SET(GIT_COMMIT "Git Not Found")
endif()
MESSAGE(STATUS "Source Version: \"" ${GIT_COMMIT} "\"")
CONFIGURE_FILE(${SRC} ${DST} @ONLY)
