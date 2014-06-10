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

typedef enum _ystring_flags_t {
    YSTRING_NONE = 0x0000, /**< no flag is set */
    YSTRING_COPY_ON_WRITE = 0x0001 /**< the buffer is */
} ystring_flags_t;

//! a logger structure
///
typedef struct _ystring_t {
    const char * buffer_;   /**< our buffer as a ycntbuff_t buffer */
    size_t bytes_used_;     /**< length of the string in bytes, including ending null */
    size_t bytes_alloc_;    /**< length of the buffer in bytes */
    int flags_;
} ystring_t;


//! may be used to add special directives to export function
/// @{
#ifndef YSTRING_EXPORT
#define YSTRING_EXPORT
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
/// Usually you do not need this function. Simply use ystring_init()
/// to an automatic variable.
///
YSTRING_EXPORT yt_func_exit_code_t
ystring_new (
        struct _ystring_t ** ystring,
        const char * value);

//! releases the memory allocated for this string using ystring_new()
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
