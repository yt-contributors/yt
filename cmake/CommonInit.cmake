include(CheckFunctionExists)


macro(CommonInit )

# ----------------------------------------------------------------------------
# definitions related to the project

project(${PROJECT_NAME})

string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_U)
string(TIMESTAMP ${PROJECT_NAME_U}_BUILD_TIME UTC)


# the version as known to CMake
# follow the Semantic Versioning 2.0.0 (http://semver.org)
set ( ${PROJECT_NAME_U}_MAJOR_VERSION 1)
set ( ${PROJECT_NAME_U}_MINOR_VERSION 0)
set ( ${PROJECT_NAME_U}_PATCH_VERSION 0)
set ( ${PROJECT_NAME_U}_VERSION
  "${${PROJECT_NAME_U}_MAJOR_VERSION}.${${PROJECT_NAME_U}_MINOR_VERSION}.${${PROJECT_NAME_U}_PATCH_VERSION}")

# ============================================================================


# ----------------------------------------------------------------------------
# set some important paths

set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${PROJECT_BINARY_DIR}/build/bin" )
set ( EXECUTABLE_OUTPUT_PATH
    "${PROJECT_BINARY_DIR}/build/bin" )
set ( LIBRARY_OUTPUT_PATH
    "${PROJECT_BINARY_DIR}/build/lib" )
set ( INCLUDE_OUTPUT_PATH
    "${PROJECT_BINARY_DIR}/build/include/${PROJECT_NAME}" )

include_directories(
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_BINARY_DIR}/build/include"
)

# Find includes in corresponding build directories
set ( CMAKE_INCLUDE_CURRENT_DIR ON)

# ============================================================================


# ----------------------------------------------------------------------------
# debug/release configuration

# may be either Release or Debug; defaulkt to Release
if ( CMAKE_BUILD_TYPE STREQUAL "" )
    set ( CMAKE_BUILD_TYPE "Release" )
endif (CMAKE_BUILD_TYPE STREQUAL "")


if   (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DQT_DEBUG=1 -D${PROJECT_NAME_U}_DEBUG=1 -D_DEBUG=1 )
    set ( DebugLogEnabled true )
else (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -DQT_NO_DEBUG=1 -D${PROJECT_NAME_U}_NO_DEBUG=1 -D_NDEBUG=1 )
    set ( DebugLogEnabled false )
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
set ( CMAKE_DEBUG_POSTFIX  "_debug")

# ============================================================================


# ----------------------------------------------------------------------------
# OS and toolchain specific hacks

# configure compiler
if ( CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)" )
    add_definitions(/W4)
else ( CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)" )
    add_definitions(-Wall)
endif()

if (WIN32)
  # set stuff for windows
  set (TARGET_SYSTEM_WIN32 ON)
else (WIN32)
  # set stuff for other systems
  set (TARGET_SYSTEM_WIN32 OFF)
endif (WIN32)

check_function_exists(snprintf HAVE_SNPRINTF)

# ============================================================================


# ----------------------------------------------------------------------------
# Qt specific settings

# Automatically link Qt executables to qtmain target on Windows
cmake_policy(SET CMP0020 OLD)

# Instruct CMake to run moc automatically when needed.
set ( CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
if (Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif(Qt5_POSITION_INDEPENDENT_CODE)

# allow the user to choose if GUI is being build
# by default, if Qt was found it is build
if (Qt5Core_FOUND)
    option(BUILD_GUI_COMPONENTS "Activate or deactivate building of GUI components"
             ON)
else (Qt5Core_FOUND)
    option(BUILD_GUI_COMPONENTS "Activate or deactivate building of GUI components"
             OFF)
endif (Qt5Core_FOUND)

# ============================================================================

# ----------------------------------------------------------------------------
# accumulate testing source files; they will be used by CommonPostInit

set (modules_testing_src)

# ============================================================================

endmacro()

