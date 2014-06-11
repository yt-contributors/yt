/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yt_func_exit_code.h
  \date			June 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef yt_func_exit_code_h_INCLUDE
#define yt_func_exit_code_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


//! defines some comonly used exit codes
///
/// The functions are free to define additional error codes
/// for their own use above FUNC_CUSTOM_BASE.
///
typedef enum _yt_func_exit_code_t {

    YT_FUNC_OK,            /**< success */

    YT_FUNC_GENERIC_ERROR, /**< not providing any other information */
    YT_FUNC_MEMORY_ERROR,  /**< failed to allocate memory */
    YT_FUNC_BAD_INPUT,     /**< bad input for function */

    YT_FUNC_CUSTOM_BASE = 1000 /**< base for custom error codes */

} yt_func_exit_code_t;


//! tell if an error code represents success or failure
///
#define yt_success(__code__) ((__code__) == YT_FUNC_OK)


//! tell if an error code represents success or failure
///
#define yt_failure(__code__) ((__code__) != YT_FUNC_OK)


//! starts a block of error aware code
///
#define yt_func_start yt_func_exit_code_t exitcode = YT_FUNC_OK; for (;;) {


//! ends a block of error aware code
///
#define yt_func_end break; }


//! returns the error code on behalf of the function
///
#define yt_func_ret return exitcode;


//! calls the function, assigns the result and checks if NULL was returned
///
#define yt_func_null(__res__, __func__) \
    __res__  = __func__; \
    if (__res__ == NULL) { exitcode = YT_FUNC_MEMORY_ERROR; break; }

//! calls the function, assigns the result and checks if YT_FUNC_OK was returned
///
#define yt_func_ok(__func__) \
    exitcode  = __func__; \
    if (exitcode != YT_FUNC_OK) break;



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
#endif // yt_func_exit_code_h_INCLUDE
