/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ydir_mkpath.c
  \date			June 2014
  \author		Nicu Tofan

  \brief		Implements ydir_mkpath function in ydir library


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

#include <yt/ymem.h>

#ifdef TARGET_SYSTEM_WIN32

#else
#   include <sys/stat.h>
#   include <sys/types.h>
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
YDIR_EXPORT yt_func_exit_code_t
YDIR_IMPLEMENT_ME ydir_mkpath(ydir_t * ydir, const char * path)
{
    // basic validation of input
    if (path == NULL) return YT_FUNC_BAD_INPUT;
    if (path[0] == 0) return YT_FUNC_BAD_INPUT;

    // see if we're doing a relative path
    int relative = ydir_path_is_relative (path);
    char * local_path = NULL;
    char * prev_end = NULL;

    yt_func_start;

    if (relative) {
        /** @todo compute absolute path */

    } else {
#ifdef TARGET_SYSTEM_WIN32
    /** @todo TARGET_SYSTEM_WIN32 */


#else
        // create a copy so that we can modify it
        yt_func_null(local_path, strdup (path));

        struct stat st;
        char c;
        int end_of_loop = 0;
        char * p_iter = local_path + 1; // skip first '/'
        for (;;) {
            // get next character
            c = *p_iter;
            if (c == 0) {
                // end of string marks a path end
                end_of_loop = 1;
            } else if (c == '/') {
                // slash marks a path end
                *p_iter = 0;
            } else {
                // anything else is skipped
                ++p_iter;
                continue;
            }
            if ((p_iter-prev_end) != 1) {

                // we have a path separator
                if (stat (local_path, &st) == 0) {
                    // exists but is not directory => error
                    if (!S_ISDIR(st.st_mode)) {
                        exitcode = YT_FUNC_BAD_INPUT;
                        break;
                    }
                } else {
                    if (0 != mkdir (local_path, 0777)) {
                        exitcode = YT_FUNC_GENERIC_ERROR;
                        break;
                    }
                }
            }

            // done?
            if (end_of_loop) break;
            prev_end = p_iter;
            *p_iter = '/';
            ++p_iter;
        }
#endif
    }
    yt_func_end;
    if (local_path != NULL) free (local_path);
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
