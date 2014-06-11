/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ybitarray.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ybitarray_h_INCLUDE
#define YT_ybitarray_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <yt/yt.h>
#include <yt/ystring.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _ybitarray_t;

//! an array of bits
///
typedef struct _ystring_t ybitarray_t;


//! may be used to add special directives to export function
/// @{
#ifndef YBITARRAY_EXPORT
#define YBITARRAY_EXPORT
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

//! initialize an empty structure
///
static inline yt_func_exit_code_t
ybitarray_init (
        ybitarray_t * ybitarray)
{
    return ystring_init_empty(ybitarray);
}

//! initialize from an array of bits
///
static inline yt_func_exit_code_t
ybitarray_init_bits (
        ybitarray_t * ybitarray,
        const void * source,
        size_t bits_count)
{
    size_t bytes_count = bits_count / 8;
    if (bytes_count * 8 != bits_count) ++bytes_count;
    return ystring_init_counted(
                ybitarray, (const char *)source, bytes_count);
}

//! initialize from an array of bytes
///
/// Bytes that are 0 are converted to 0 bits, those that
/// are not are converted to 1 bits.
///
YBITARRAY_EXPORT yt_func_exit_code_t
ybitarray_init_bytes (
        ybitarray_t * ybitarray,
        const void * source,
        size_t bytes_count);

//! terminate
///
static inline void
ybitarray_end (
        ybitarray_t * ybitarray)
{
    ystring_end (ybitarray);
}

///@}
// == == == == == == == == == == == == == == == == == == == == == ==

// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Get and set
 *  Operates on individual bits
 */
///@{


//! set the value of a bit
///
YBITARRAY_EXPORT yt_func_exit_code_t
ybitarray_set (
        ybitarray_t * ybitarray,
        size_t offset,
        int value);

//! get the value of a bit
///
/// If \b offset is outside allowable range 0 is returned.
///
YBITARRAY_EXPORT int
ybitarray_get (
        ybitarray_t * ybitarray,
        size_t offset);


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
#endif // YT_ybitarray_h_INCLUDE
