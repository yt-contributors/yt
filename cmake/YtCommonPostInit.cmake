macro(YtCommonPostInit )


# ----------------------------------------------------------------------------
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${INCLUDE_OUTPUT_PATH}/config.h"
)
# ============================================================================


# ----------------------------------------------------------------------------
# internal tests
if (YT_BUILD_TESTS)
    add_subdirectory( "googletest" )
    include_directories(googletest/include)

    add_subdirectory( "tests" )
    if (modules_testing_src)
        list( REMOVE_DUPLICATES modules_testing_src )
        add_executable(internal-test
            ${modules_testing_src}
        )
        target_link_libraries ( internal-test
            gtest_main gtest
            ${test_libraries}
        )
        set ( props "${PROJECT_NAME_U}_INTERNAL_TESTS=1")
        set_target_properties ( internal-test
            PROPERTIES COMPILE_DEFINITIONS "${props}"
        )
    endif (modules_testing_src)
endif (YT_BUILD_TESTS)
# ============================================================================


# ----------------------------------------------------------------------------
# documentation
if(EXISTS "${PROJECT_SOURCE_DIR}/Doxyfile.in")

	find_package(Doxygen)

	configure_file (
		"${PROJECT_SOURCE_DIR}/Doxyfile.in"
		"${PROJECT_BINARY_DIR}/Doxyfile"
		@ONLY
	)

	if(DOXYGEN_FOUND)
		add_custom_target(doc
			${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
	endif(DOXYGEN_FOUND)
endif(EXISTS "${PROJECT_SOURCE_DIR}/Doxyfile.in")
# ============================================================================


endmacro()
