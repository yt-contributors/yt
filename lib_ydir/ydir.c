/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ydir.c
  \date			June 2014
  \author		Nicu Tofan

  \brief


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

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_init (ydir_t * ydir)
    //{
    //    memset (ydir, 0, sizeof(ydir_t));
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_init_dir (ydir_t * ydir, const char * dir)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT void
    //ydir_end (ydir_t * ydir)
    //{
    //    if (ydir->absolute_path_ != NULL) free (ydir->absolute_path_);
    //    memset (ydir, 0, sizeof(ydir_t));
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_set_dir (ydir_t * ydir, const char * absolute_path)
    //{

    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_cd (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_file_exists (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_dir_exists (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT int
    //ydir_path_is_relative (const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT int
    //ydir_is_root (const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT int
    //ydir_i_exist (const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_mkdir (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_mkpath (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_relative (ydir_t * ydir, const char * absolute_path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_remove (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_remove_dir (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_remove_file (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_copy (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_move (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT int
    //ydir_foreach (
    //        ydir_t * ydir, const char * pattern, ydir_iter_kb kb, void * user)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT int
    //ydir_foreach_dir (
    //        ydir_t * ydir, const char * pattern, ydir_iter_kb kb, void * user)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT int
    //ydir_foreach_file (
    //        ydir_t * ydir, const char * pattern, ydir_iter_kb kb, void * user)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT yt_func_exit_code_t
    //ydir_set_current (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT int
    //ydir_is_readable (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT int
    //ydir_is_writable (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */

    ///* ------------------------------------------------------------------------- */
    //YDIR_EXPORT int
    //ydir_is_executable (ydir_t * ydir, const char * path)
    //{
    //    return YT_FUNC_OK;
    //}
    ///* ========================================================================= */


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */