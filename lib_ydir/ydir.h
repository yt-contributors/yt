/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ydir.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef MOCAL_ydir_args_h_INCLUDE
#define MOCAL_ydir_args_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <yt/yt.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! a directory
///
typedef struct _ydir_t {

    char * internal_path_;

} ydir_t;

#ifdef TARGET_SYSTEM_WIN32
#    define YDIR_PATH_SEP_C '\\'
#    define YDIR_PATH_SEP "\\"
#else
#    define YDIR_PATH_SEP_C '/'
#    define YDIR_PATH_SEP "/"
#endif

#ifndef YDIR_EXPORT
#define YDIR_EXPORT
#endif

#ifndef YDIR_IMPLEMENT_ME
#define YDIR_IMPLEMENT_ME
#endif

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

//! initialize
///
YDIR_EXPORT yt_func_exit_code_t
ydir_init (
        struct _ydir_t * ydir);

//! initialize
///
YDIR_EXPORT yt_func_exit_code_t
ydir_init_dir (
        struct _ydir_t * ydir,
        const char * dir);

//! terminate
///
YDIR_EXPORT void
ydir_end (
        struct _ydir_t * ydir);

//! set internal path; must be an absolute path
///
YDIR_EXPORT yt_func_exit_code_t
ydir_set_dir (
        struct _ydir_t * ydir,
        const char * absolute_path);

//! append new path to current path
///
/// If the \b path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
/// If \b ydir is NULL YT_FUNC_BAD_INPUT is returned.
/// if \b path is NULL that value is saved and YT_FUNC_OK is returned.
///
YDIR_EXPORT yt_func_exit_code_t
ydir_cd (
        struct _ydir_t * ydir,
        const char * path);

//! tell if there is a file at the orther end of the path
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT int
ydir_file_exists (
        struct _ydir_t * ydir,
        const char * path);

//! tell if there is a file at the orther end of the path
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT int
ydir_dir_exists (
        struct _ydir_t * ydir,
        const char * path);


//! tell if the path is absolute or relative
///
/// For windows machines paths where second character is ':' and
/// those that start with '//' are absolute.
///
/// For posix machines paths that start with a '/' are absolute.
///
/// NULL and zero-length strings result in absolute paths.
///
/// @return 0 for absolute paths, 1 for relative paths
///
YDIR_EXPORT int
ydir_path_is_relative (
        const char * path);


//! tell if this structure represents the root directory
///
YDIR_EXPORT int
ydir_is_root (
        const char * path);

//! tell if the directory represented by this structure exist
///
/// This is the same as calling ydir_dir_exists(ydir, ".")
///
YDIR_EXPORT int
ydir_i_exist (
        const char * path);

//! create a sub-directory
///
/// Fails if the directory already exists. Also fails if any component
/// of the path except last one does not exists.
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT yt_func_exit_code_t
ydir_mkdir (
        struct _ydir_t * ydir,
        const char * path);

//! create a sub-directory; fails if it already exists
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT yt_func_exit_code_t
ydir_mkpath (
        struct _ydir_t * ydir,
        const char * path);

//! create a symbolic link
///
/// If the any path is relative (ydir_path_is_relative() returns 1)
/// internal path is used to compute an absolute path.
///
YDIR_EXPORT yt_func_exit_code_t
ydir_symlink (
        struct _ydir_t * ydir,
        const char * path_src,
        const char * path_dest);

//! compute the path relative to our directory
///
YDIR_EXPORT yt_func_exit_code_t
ydir_relative (
        struct _ydir_t * ydir,
        const char * absolute_path);

//! removes the sub-directory or the file
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT yt_func_exit_code_t
ydir_remove (
        struct _ydir_t * ydir,
        const char * path);

//! removes the sub-directory;
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT yt_func_exit_code_t
ydir_remove_dir (
        struct _ydir_t * ydir,
        const char * path);

//! removes the file;
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT yt_func_exit_code_t
ydir_remove_file (
        struct _ydir_t * ydir,
        const char * path);

//! copies our directory (and all its content) to a different location
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT yt_func_exit_code_t
ydir_copy (
        struct _ydir_t * ydir,
        const char * path);

//! moves our directory (and all its content) to a different location
///
/// Internal path IS NOT change to new location; use ydir_cd() for that.
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT yt_func_exit_code_t
ydir_move (
        struct _ydir_t * ydir,
        const char * path);


//! callback used to iterate in items from a path
///
/// @return non-zero to terminate the loop
///
typedef int (*ydir_iter_kb) (
        struct _ydir_t * ydir,
        const char * absolute_path,
        int is_file,
        void * user);

//! calls the callback for each file or directory
///
YDIR_EXPORT int
ydir_foreach (
        struct _ydir_t * ydir,
        const char * pattern,
        ydir_iter_kb kb,
        void * user);

//! calls the callback for each directory
///
YDIR_EXPORT int
ydir_foreach_dir (
        struct _ydir_t * ydir,
        const char * pattern,
        ydir_iter_kb kb,
        void * user);

//! calls the callback for each file
///
YDIR_EXPORT int
ydir_foreach_file (
        struct _ydir_t * ydir,
        const char * pattern,
        ydir_iter_kb kb,
        void * user);

//! sets current directory for the application
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT yt_func_exit_code_t
ydir_set_current (
        struct _ydir_t * ydir,
        const char * path);

//! tell if the item exists and is a directory or something else
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path.
///
YDIR_EXPORT int
ydir_is_directory(
        struct _ydir_t * ydir,
        const char * path);

//! tell if the item (file or directory) exists and is readable by this process
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT int
ydir_is_readable(
        struct _ydir_t * ydir,
        const char * path);

//! tell if the item (file or directory) exists and is writable by this process
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT int
ydir_is_writable(
        struct _ydir_t * ydir,
        const char * path);

//! tell if the item (file or directory) exists and is executable by this process
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT int
ydir_is_executable(
        struct _ydir_t * ydir,
        const char * path);

//! the path separator
///
static inline char
ydir_path_separator ()
{ return YDIR_PATH_SEP_C; }

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifdef __cplusplus
}
#endif
#endif // MOCAL_ydir_args_h_INCLUDE
