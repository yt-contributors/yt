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

//! initialize a structure from an utf8, null terminated string
///
/// Prepares the structure; initializes the string with an
/// initial value.
///
YSTRING_EXPORT yt_func_exit_code_t
ystring_init (
        struct _ystring_t * ystring,
        const char * value);

//! initialize a structure from a counted string
///
/// Prepares the structure; initializes the string with an
/// initial value.
///
YSTRING_EXPORT yt_func_exit_code_t
ystring_init_counted (
        struct _ystring_t * ystring,
        const char * value,
        size_t value_len);

//! initialize an empty structure
///
#define ystring_init_empty(__addr__) ystring_init((__addr__), NULL)

//! initialize a structure from another structure
///
/// Prepares the structure; initializes the string with an
/// initial value.
///
YSTRING_EXPORT yt_func_exit_code_t
ystring_init_c (
        struct _ystring_t * ystring,
        struct _ystring_t * ystring_source);

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


//! make sure that internal buffer is at least that long
///
YSTRING_EXPORT yt_func_exit_code_t
ystring_reserve (
        struct _ystring_t * ystring,
        size_t new_len);

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Basic querries
 * Functions to check the state of the string. A NULL state
 * can be differentiated from a zero-size string or can be treated
 * just the same.
 */
///@{

//! tell if there is a buffer allocated or not
///
static inline int
ystring_is_null (
        struct _ystring_t * ystring)
{
    return (ystring->buffer_ == NULL);
}

//! tell if this is a zero-length string or is null
///
static inline int
ystring_is_empty (
        struct _ystring_t * ystring)
{
    return (ystring->buffer_ == NULL) || (ystring->bytes_used_ == 0);
}

//! the length of the string in bytes
///
/// As with strlen(), the result does not include terminating null character
///
static inline int
ystring_len (
        struct _ystring_t * ystring)
{
    return (ystring->bytes_used_ > 0 ? ystring->bytes_used_ - 1 : 0);
}

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name change the string
 * Here are functions to alter the content.
 */
///@{

//! clear the string
///
/// Resulted string always responds true to ystring_is_empty() and it
/// mat sometimes respond to ystring_is_null().
///
YSTRING_EXPORT void
ystring_clear (
        struct _ystring_t * ystring);

//! set the value from an utf8, null terminated string
///
/// Potentially, this opperation is cheap, if the buffer is not shared
/// with other instances. If it is then we give up our reference
/// and create a new buffer for ourselves.
///
YSTRING_EXPORT yt_func_exit_code_t
ystring_set (
        struct _ystring_t * ystring,
        const char * value);

//! set the value from another string
///
/// This is a cheap opperation as underlying buffer is shared.
/// The only way the function may fail is due to bad input.
///
YSTRING_EXPORT yt_func_exit_code_t
ystring_copy (
        struct _ystring_t * ystring_from,
        struct _ystring_t * ystring_to);

//! appends all string into first one
///
/// The function may fail due to memory allocation errors or bad input.
/// No matter the number of the strings to append, a single memory
/// opperation is performd. Thus, it is way more efficiently to do a single
/// append opperation.
///
YSTRING_EXPORT yt_func_exit_code_t
ystring_append (
        struct _ystring_t * ystring_result,
        size_t count,
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
#endif // YT_ystring_h_INCLUDE
