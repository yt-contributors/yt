# ----------------------------------------------------------------------------
# Introduction
#
# The project uses cmake to build its components. This is the main cmake file
# that should be used like so:
#     mkdir build && cd build && cmake $OPTIONS ../src && make && make install
#
# $OPTIONS are a set of definitions of the form `-DDEFINITION=Value`
# Here are valid definitions:
#
#   -DCMAKE_BUILD_TYPE=Debug
#   -DCMAKE_BUILD_TYPE=Release
#       Either enable or disable development aids;
#
#   -DCMAKE_INSTALL_PREFIX:PATH=/usr
#       The base path for installed components;
#
#   -DBUILD_TESTS=ON
#       Activate or deactivate building of embedded google test
#       suite and internal tests
#
#
# To get the full list of make targets run `make help` in build directory. Here
# are some important ones:
#
#   install
#       copies installable components to their intended places
#
#   doc
#       creates documentation using Doxygen
#
#   internal-test
#       creates internal test executable based on Google Test suite
#
# Dependencies:
#
#
# ============================================================================


# ----------------------------------------------------------------------------
# prepare

cmake_minimum_required(VERSION 2.8.9)
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(CommonInit)
include(CommonPostInit)
# ============================================================================


# ----------------------------------------------------------------------------
# customisation

set ( PROJECT_NAME "yt" )

option(BUILD_TESTS "Activate or deactivate building of embedded google test suite and internal tests"
         OFF)

# ============================================================================


# ----------------------------------------------------------------------------
# prepare the cmake system with some common settings

CommonInit()

if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set ( YT_DEBUG ON )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set ( YT_DEBUG OFF)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
# ============================================================================


# ----------------------------------------------------------------------------
# external libraries

find_package(Syslog)
include_directories(${SYSLOG_INCLUDE_DIRS})
set (YT_SYSLOG_FOUND SYSLOG_FOUND)

# ============================================================================


# ----------------------------------------------------------------------------
# subdirectories

add_subdirectory("common")


set (Y_LIB_LIST
    ylogger
    ybitarray
    ybytearray
    ydatetime
    ydir
    yfile
    yhash
    yline
    ylinef
    ylinkedlist
    ylist
    ymap
    ymargins
    ymarginsf
    ymutex
    yobject
    ypair
    ypoint
    ypointf
    yqueue
    yrect
    yrectf
    yset
    ysize
    ysizef
    ystack
    ystring
    ystringlist
    yurl
    yutf8
    yuuid
    yvariant
    yvector
    ymem
)

FOREACH(iterlib ${Y_LIB_LIST})
    ADD_SUBDIRECTORY("lib_${iterlib}")
ENDFOREACH()

# ============================================================================


# ----------------------------------------------------------------------------
# other cmake system common settings

# the list of libraries to be linked with test executable
set (test_libraries
    ${Y_LIB_LIST}
)

CommonPostInit()

# ============================================================================
