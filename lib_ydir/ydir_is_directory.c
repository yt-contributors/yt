/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ydir_is_directory.c
  \date			June 2014
  \author		Nicu Tofan

  \brief		Implements ydir_is_directory function in ydir library


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "ydir.h"

#include <string.h>

#if TARGET_SYSTEM_WIN32

#else
#   include <sys/stat.h>
#endif

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
YDIR_EXPORT int
YDIR_IMPLEMENT_ME ydir_is_directory (ydir_t * ydir, const char * path)
{
    // no path does not exists
    if (path == NULL) return 0;

#if TARGET_SYSTEM_WIN32
    /** @todo TARGET_SYSTEM_WIN32 */

#else
    struct stat st;

    // do we need to compute absolute path or we're good?
    if (ydir_path_is_relative (path)) {
        if (ydir == NULL) {
            // no base path for relative input; this is an error but
            // we have no way of reporting it
            return 0;
        }

        /** @todo compute absolute path */

    } else {
        if (stat (path, &st) == 0) {
            return S_ISDIR(st.st_mode);
        } else {
            return 0;
        }
    }
#endif
    return 0;
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
