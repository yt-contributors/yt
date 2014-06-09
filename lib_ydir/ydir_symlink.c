/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ydir_symlink.c
  \date			June 2014
  \author		Nicu Tofan

  \brief		Implements ydir_symlink function in ydir library


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "ydir.h"

#include <yt/ylogger.h>

#include <errno.h>
#include <unistd.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */
YDIR_EXPORT yt_func_exit_code_t
YDIR_IMPLEMENT_ME ydir_symlink(
        ydir_t * ydir, const char * path_src, const char * path_dest)
{

    // no path does not exists
    if ((path_src == NULL) || (path_src == NULL)) {
        return YT_FUNC_BAD_INPUT;
    }

    yt_func_start;

    // cache relativity
    int src_rel = ydir_path_is_relative (path_src);
    int dest_rel = ydir_path_is_relative (path_dest);

#if TARGET_SYSTEM_WIN32
    /** @todo TARGET_SYSTEM_WIN32 */

#else
    if (src_rel) {
        if (dest_rel) {
            /** @todo relativity */
        } else {
            /** @todo relativity */
        }
    } else {
        if (dest_rel) {
            /** @todo relativity */
        } else {
             if (symlink (path_src, path_dest) != 0) {
                 err_message (
                             "Could not create symbolic link %s "
                             "towards %s; error %d",
                             path_dest,
                             path_src,
                             errno);
                 exitcode = YT_FUNC_GENERIC_ERROR;
                 break;
             }
        }
    }

#endif

    yt_func_end;
    yt_func_ret;
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
