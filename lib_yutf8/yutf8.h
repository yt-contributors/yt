/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yutf8.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_yutf8_h_INCLUDE
#define YT_yutf8_h_INCLUDE
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

struct _yutf8_t;

//! a logger structure
///
typedef struct _yutf8_t {

    const char * value_;     /**< the string as an array of utf8 characters */

} yutf8_t;


//! may be used to add special directives to export function
/// @{
#ifndef YUTF8_EXPORT
#define YUTF8_EXPORT
#endif
/// @}


//! not yet implemented
/// @{
#ifndef YUTF8_IMPLEMENT_ME
#define YUTF8_IMPLEMENT_ME
#endif
/// @}


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
/// Sets the pointer; no copy is performed; the ownership stays with the caller.
///
YUTF8_EXPORT yt_func_exit_code_t
yutf8_init (
        struct _yutf8_t * yutf8,
        const char * value);

//! terminate
///
/// Internal pointer is not freed.
///
YUTF8_EXPORT void
yutf8_end (
        struct _yutf8_t * yutf8);

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name New strings from old string
 *  Functions that create new things
 */
///@{

//! join two or more strings
///
/// Allocates a new buffer with the size of the inputs +1 and copies them there.
///
/// YT_FUNC_BAD_INPUT is returned if \b out is NULL.
/// YT_FUNC_MEMORY_ERROR is returned in case of memory allocation failures.
///
YUTF8_EXPORT yt_func_exit_code_t
yutf8_join (
        struct _yutf8_t * out,
        size_t count,
        ...);

//! join two or more strings with a separator between them
///
/// Allocates a new buffer with the size of the inputs +1 and copies them there.
///
/// YT_FUNC_BAD_INPUT is returned if \b out is NULL.
/// YT_FUNC_MEMORY_ERROR is returned in case of memory allocation failures.
///
YUTF8_EXPORT yt_func_exit_code_t
yutf8_join_sep (
        const char * separator,
        struct _yutf8_t * out,
        size_t count,
        ...);

//! prints formatted string to an internally allocated buffer
///
/// Computes the length of the string, allocates the buffer and prints
/// the string.
///
/// YT_FUNC_BAD_INPUT is returned if \b out is NULL.
/// YT_FUNC_MEMORY_ERROR is returned in case of memory allocation failures.
///
YUTF8_EXPORT yt_func_exit_code_t
yutf8_sprintf (
        struct _yutf8_t * out,
        const char * format,
        ...);

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
#endif // YT_yutf8_h_INCLUDE
