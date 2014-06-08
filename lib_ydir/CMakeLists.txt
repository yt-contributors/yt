# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# ============================================================================

# ----------------------------------------------------------------------------
# options

# ============================================================================

# ----------------------------------------------------------------------------
# configure the project

set ( LOC_PRJ_NAME "ydir" )

# ============================================================================

# ----------------------------------------------------------------------------
# our library
set ( LIB_SOURCES
    ydir.c
    ydir_cd.c
    ydir_copy.c
    ydir_dir_exists.c
    ydir_end.c
    ydir_file_exists.c
    ydir_foreach.c
    ydir_foreach_dir.c
    ydir_foreach_file.c
    ydir_i_exist.c
    ydir_init_dir.c
    ydir_init.c
    ydir_is_executable.c
    ydir_is_readable.c
    ydir_is_root.c
    ydir_is_writable.c
    ydir_mkdir.c
    ydir_mkpath.c
    ydir_move.c
    ydir_path_is_relative.c
    ydir_relative.c
    ydir_remove_dir.c
    ydir_remove_file.c
    ydir_remove.c
    ydir_set_current.c
    ydir_set_dir.c
)
set ( LIB_HEADERS
    ydir.h
)
set (LIBS
    "ymem_malloc"
)

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

