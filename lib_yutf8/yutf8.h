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
#include <stdint.h>

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


//! special values for bytes
///
typedef enum _yutf8_special_t {

    YUTF8_FIRST_BIT  = 128, /**< first byte > 127 => beyond the ASCII range. */
    YUTF8_SECOND_BIT = 64,  /**<  */
    YUTF8_THIRD_BIT  = 32,  /**< first byte > 224 => at least a three-octet code point. */
    YUTF8_FOURTH_BIT = 16,  /**< first byte > 240 => four-octet code point */
    YUTF8_FIFTH_BIT  = 8,   /**<  */

} yutf8_special_t;


const unsigned char kFirstBitMask = 128; // 1000000
const unsigned char kSecondBitMask = 64; // 0100000
const unsigned char kThirdBitMask = 32; // 0010000
const unsigned char kFourthBitMask = 16; // 0001000
const unsigned char kFifthBitMask = 8; // 0000100

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


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Iterators
 *  Passing through all code points in an utf8 string
 */
///@{

//! callback function used to iterate utf8 code points ("letters")
///
typedef yt_func_exit_code_t (*yutf8_foreach_kb) (
        struct _yutf8_t * yutf8,
        uint32_t code_point,
        size_t offset,
        size_t index,
        void * user
        );

//! iterator; passes through all code points
///
/// If the callback returns anything else than YT_FUNC_OK the
/// loop is terminated early and that particular value is returned.
///
YUTF8_EXPORT yt_func_exit_code_t
yutf8_foreach (
        struct _yutf8_t * yutf8,
        yutf8_foreach_kb kb,
        void * user);


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
