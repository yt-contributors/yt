/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ystring.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ystring_h_INCLUDE
#define YT_ystring_h_INCLUDE
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

struct _ystring_t;

//! a logger structure
///
typedef struct _ystring_t {
    const char * buffer_;   /**< our buffer */
    size_t bytes_used_;     /**< length of the string in bytes */
    size_t bytes_alloc_;    /**< length of the buffer in bytes */
} ystring_t;


//! may be used to add special directives to export function
/// @{
#ifndef YSTRING_EXPORT
#define YSTRING_EXPORT
#endif
/// @}


//! not yet implemented
/// @{
#ifndef YSTRING_IMPLEMENT_ME
#define YSTRING_IMPLEMENT_ME
#endif
/// @}


//! simple functions are outside the namespace by default
/// @{
#ifdef YSTRING_WRAP_ALL
#define YSTRING_WRAP(__name__) ystring_ ## __name__
#else
#define YSTRING_WRAP(__name__) __name__
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
/// Prepares the structure; initializes the string with an
/// initial value.
///
YSTRING_EXPORT yt_func_exit_code_t
ystring_init (
        struct _ystring_t * ystring,
        const char * value);

//! terminate
///
YSTRING_EXPORT void
ystring_end (
        struct _ystring_t * ystring);

//! allocate a new string
///
/// Prepares the structure; initializes the string with an
/// initial value.
///
YSTRING_EXPORT yt_func_exit_code_t
ystring_new (
        struct _ystring_t ** ystring,
        const char * value);

//! releases the memory
///
YSTRING_EXPORT void
ystring_free (
        struct _ystring_t ** ystring);


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
#endif // YT_ystring_h_INCLUDE
