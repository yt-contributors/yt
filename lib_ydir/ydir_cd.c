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
#include <yt/ymem.h>

#include <string.h>

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
YDIR_IMPLEMENT_ME ydir_cd (ydir_t * ydir, const char * path)
{
    if (ydir == NULL) return YT_FUNC_BAD_INPUT;
    char * old_value = ydir->internal_path_;

    // special case - NULL
    if (path == NULL) {
        if (old_value != NULL) ymem_free(old_value);
        ydir->internal_path_ = NULL;
        return YT_FUNC_OK;
    }

    yt_func_start;

    if (ydir_path_is_relative (path)) {

    } else {
        yt_func_null(ydir->internal_path_, ymem_strdup(path));

    }
    if (old_value != NULL) ymem_free(old_value);

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
