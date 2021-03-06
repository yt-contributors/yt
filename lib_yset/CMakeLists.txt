# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# ============================================================================

# ----------------------------------------------------------------------------
# options

# ============================================================================

# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "yset" )

# ============================================================================

# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    yset.c
)
set ( LIB_HEADERS
    yset.h
)
set (LIBS "ylogger")


add_library( ${LOC_PRJ_NAME}
    ${LIB_SOURCES}
)

target_link_libraries( ${LOC_PRJ_NAME}
    ${LIBS}
)

if (INCLUDE_OUTPUT_PATH)
    FOREACH (hfile ${LIB_HEADERS})
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${hfile}" "${INCLUDE_OUTPUT_PATH}/${hfile}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
    ENDFOREACH(hfile)
endif (INCLUDE_OUTPUT_PATH)
# ============================================================================

