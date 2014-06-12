/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ydir_cd.c
  \date			June 2014
  \author		Nicu Tofan

  \brief		Implements ydir_cd function in ydir library


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "ydir.h"
#include "ydir_internal.h"

#include <string.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define IS_SEPARATOR(__c__) ((__c__ == 0) || (__c__ == '/') || (__c__ == '\\') || (__c__ == YDIR_PATH_SEP_C))
#ifdef TARGET_SYSTEM_WIN32
#define ROOT_LENGTH 3
#else
#define ROOT_LENGTH 1
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

/* ------------------------------------------------------------------------- */
yt_func_exit_code_t
ydir_cd_buff (
        const char * base_path, size_t base_path_len,
        const char * new_path, size_t new_path_len,
        char * out_path, size_t out_allocated_len)
{
    DBG_ASSERT (base_path != NULL);;
    if (base_path_len == 0) {
        base_path_len = strlen (base_path);
    }

    DBG_ASSERT (new_path != NULL);;
    if (new_path_len == 0) {
        new_path_len = strlen (new_path);
    }

    if (out_allocated_len < base_path_len + new_path_len + 1) {
        return YT_FUNC_BAD_INPUT;
    }

    DBG_ASSERT (ydir_path_is_relative (new_path));;

    // copy base inside our buffer
    if (base_path_len <= ROOT_LENGTH) {
        base_path_len = ROOT_LENGTH-1;
    }
    char * out_path_iter = out_path + base_path_len;
    const char * new_path_iter = new_path;
    memcpy (out_path, base_path, base_path_len);
    size_t i;
    char c;
    char c2;
    for (i = 0; i < new_path_len; ++i) {

        // a path component starts here
        c = *new_path_iter++;
        if (c == '.') {
            c2 = new_path_iter[0];
            if (c2 == '.') {
                if (IS_SEPARATOR(new_path_iter[1])) {
                    if ((out_path_iter - out_path) <= ROOT_LENGTH) return YT_FUNC_BAD_INPUT;

                    // we have a ..
                    while (out_path_iter != out_path) {
                        c = *(--out_path_iter);
                        if (IS_SEPARATOR(c)) break;
                    }

                    new_path_iter += 2;
                    i += 2;
                    continue;
                }
            } else if (IS_SEPARATOR(c2)) {
                // we have a .
                new_path_iter += 1;
                ++i;
                continue;
            }
        }

        *out_path_iter = YDIR_PATH_SEP_C;
        ++out_path_iter;

        // we're inside a path component
        for (; i < new_path_len; ) {
            *out_path_iter = c;
            ++out_path_iter;
            c = *new_path_iter++;
            ++i;
            if (IS_SEPARATOR(c)) break;
        }
    }
    if (out_path_iter == out_path) {
#ifdef TARGET_SYSTEM_WIN32
        return YT_FUNC_BAD_INPUT;
#else
        *out_path_iter = YDIR_PATH_SEP_C;
        ++out_path_iter;
#endif
    }
    *out_path_iter = 0;

    return YT_FUNC_OK;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YDIR_EXPORT yt_func_exit_code_t
YDIR_IMPLEMENT_ME ydir_cd (ydir_t * ydir, const char * path)
{
    if (ydir == NULL) return YT_FUNC_BAD_INPUT;

    // special case - NULL
    if (path == NULL) {
        ystring_clear (&ydir->path_);
        return YT_FUNC_OK;
    }

    yt_func_start;

    IF_IS_RELATIVE_DO(ydir, path);
        exitcode = ystring_set (&ydir->path_, absolute_path_ptr);
    IF_IS_NOT_RELATIVE_DO;
        exitcode = ystring_set (&ydir->path_, path);
    END_IF_RELATIVE;

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
