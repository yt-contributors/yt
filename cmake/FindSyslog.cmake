# - Try to find syslog header
# Once done this will define
#  SYSLOG_FOUND - System has syslog
#  SYSLOG_INCLUDE_DIRS - include directories


find_path(SYSLOG_INCLUDE_DIR syslog.h
          PATHS ENV SYSLOG_INC )

set(SYSLOG_INCLUDE_DIRS ${ARGTABLE_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set SYSLOG_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Syslog  DEFAULT_MSG
                                  SYSLOG_INCLUDE_DIR)

mark_as_advanced(SYSLOG_INCLUDE_DIR)

