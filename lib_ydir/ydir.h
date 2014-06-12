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
#include <yt/ystring.h>

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
    ystring_t path_;
#   define internal_path_    path_.buffer_
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

// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Base
 *  Constructors, destructors and so on
 */
///@{

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

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name change the directory
 * Here are functions to alter the content.
 */
///@{

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

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Basic querries
 * We're checking for all sorts of stuff here
 */
///@{

//! the path separator
///
static inline char
ydir_path_separator ()
{ return YDIR_PATH_SEP_C; }

//! the absolute path stored inside
///
/// The pointer is valid until next operation that changes the
/// path.
///
static inline const char *
ydir_path (
        struct _ydir_t * ydir)
{ return ydir == NULL ? NULL : ydir->path_.buffer_; }

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

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Creating things
 * This includes individual directories, entire paths and so on
 */
///@{

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

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Remove things things
 *
 */
///@{

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

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Various
 *
 */
///@{


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


//! compute the path relative to our directory
///
YDIR_EXPORT yt_func_exit_code_t
ydir_relative (
        struct _ydir_t * ydir,
        const char * absolute_path);


//! sets current directory for the application
///
/// If the path is relative (ydir_path_is_relative() returns 1) internal path is used
/// to compute an absolute path
///
YDIR_EXPORT yt_func_exit_code_t
ydir_set_current (
        struct _ydir_t * ydir,
        const char * path);

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Iterators
 * Client code can iterate in all sorts of items. The pattern can
 * contain YDIR_PATTERN_CHARACTER and/or YDIR_PATTERN_GROUP. Do not
 * make any other assumptions about its internal workings as they
 * may be subject to change.
 */
///@{

#ifdef TARGET_SYSTEM_WIN32
//! pattern to match any individual character
#   define YDIR_PATTERN_CHARACTER "?"
//! pattern to match a group of characters
#   define YDIR_PATTERN_GROUP "*"
#else
//! pattern to match any individual character
#   define YDIR_PATTERN_CHARACTER "."
//! pattern to match a group of characters
#   define YDIR_PATTERN_GROUP ".*"
#endif

//! type filter flags
typedef enum _ydir_iter_flags_t{
    YDIR_ITER_FILES_AND_DIRECTORIES = 0,
    YDIR_ITER_EXCLUDE_DIRECTORIES = 0x0001,   /**< no dirs reach the callback */
    YDIR_ITER_EXCLUDE_FILES = 0x0002,   /**< no files reach the callback */
    YDIR_ITER_ALL_DIRECTORIES = 0x0004, /**< don't use the pattern for directories, dump all to the callback */
    YDIR_ITER_ALL_FILES = 0x0008,       /**< don't use the pattern for files, dump all to the callback */

    YDIR_ITER_RECURSIVE = 0x0010
} ydir_iter_flags_t;


//! callback used to iterate in items from a path
///
/// @return non-zero to terminate the loop
///
typedef yt_func_exit_code_t (*ydir_iter_kb) (
        struct _ydir_t * ydir,
        const char * absolute_path,
        const char * name_only,
        int is_file,
        void * user);

//! calls the callback for each file or directory
///
/// This is the most general form, allowing to customise all parameters.
/// Other functions will call this function with appropriate switches.
///
YDIR_EXPORT yt_func_exit_code_t
ydir_foreach (
        struct _ydir_t * ydir,
        const char * pattern,
        int flags,
        ydir_iter_kb kb,
        void * user);

//! calls the callback for each top level directory
///
YDIR_EXPORT yt_func_exit_code_t
ydir_foreach_top_dir (
        struct _ydir_t * ydir,
        const char * pattern,
        ydir_iter_kb kb,
        void * user);

//! calls the callback for each directory in all subdirectories
///
YDIR_EXPORT yt_func_exit_code_t
ydir_foreach_dir (
        struct _ydir_t * ydir,
        const char * pattern,
        ydir_iter_kb kb,
        void * user);

//! calls the callback for each file inside the directory
///
YDIR_EXPORT yt_func_exit_code_t
ydir_foreach_top_file (
        struct _ydir_t * ydir,
        const char * pattern,
        ydir_iter_kb kb,
        void * user);

//! calls the callback for each file in all subdirectories
///
YDIR_EXPORT yt_func_exit_code_t
ydir_foreach_file (
        struct _ydir_t * ydir,
        const char * pattern,
        ydir_iter_kb kb,
        void * user);

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


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
