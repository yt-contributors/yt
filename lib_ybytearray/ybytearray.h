/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ybytearray.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ybytearray_h_INCLUDE
#define YT_ybytearray_h_INCLUDE
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

struct _ybytearray_t;

//! a logger structure
///
typedef struct _ybytearray_t {

    const char * app_name_;     /**< the reported name of the application */

} ybytearray_t;


//! may be used to add special directives to export function
/// @{
#ifndef ybytearray_EXPORT
#define ybytearray_EXPORT
#endif
/// @}


//! not yet implemented
/// @{
#ifndef ybytearray_IMPLEMENT_ME
#define ybytearray_IMPLEMENT_ME
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
/// Provides the structure with default callbacks.
///
ybytearray_EXPORT yt_func_exit_code_t
ybytearray_init (
        struct _ybytearray_t * ybytearray,
        const char * app_name);

//! terminate
///
ybytearray_EXPORT void
ybytearray_end (
        struct _ybytearray_t * ybytearray);

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
#endif // YT_ybytearray_h_INCLUDE
