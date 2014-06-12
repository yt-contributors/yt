/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ydir_internal.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef MOCAL_ydir_internal_h_INCLUDE
#define MOCAL_ydir_internal_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <yt/yt.h>
#include <yt/ymem.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define IF_IS_RELATIVE_DO(__ydir__, __relative_path__) \
    if (ydir_path_is_relative (__relative_path__)) { \
        size_t new_len = strlen (__relative_path__); \
        YT_STACKBUFF_INIT(absolute_path, char, 512, new_len + __ydir__->path_.bytes_used_); \
        yt_func_null(absolute_path_ptr, absolute_path_ptr); \
        \
        exitcode = ydir_cd_buff( \
                    __ydir__->path_.buffer_, __ydir__->path_.bytes_used_-1, \
                    path, new_len, \
                    absolute_path_ptr, absolute_path_actual_sz \
                    ); \
        if (yt_success(exitcode)) {

#define IF_IS_NOT_RELATIVE_DO \
        } \
        YT_STACKBUFF_END(absolute_path); \
    } else {

#define END_IF_RELATIVE \
    }

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

//!
//! \brief converts a base + relative path into an absolute path
//! \param base_path
//! \param base_path_len
//! \param new_path
//! \param new_path_len
//! \param out_path
//! \param out_allocated_len
//! \return
//!
yt_func_exit_code_t
ydir_cd_buff (
        const char * base_path, size_t base_path_len,
        const char * new_path, size_t new_path_len,
        char * out_path, size_t out_allocated_len);

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
#endif // MOCAL_ydir_internal_h_INCLUDE
