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

//! a logger structure
///
typedef struct _ybitarray_t {

    const char * app_name_;     /**< the reported name of the application */

} ybitarray_t;


//! may be used to add special directives to export function
/// @{
#ifndef YBITARRAY_EXPORT
#define YBITARRAY_EXPORT
#endif
/// @}


//! not yet implemented
/// @{
#ifndef ybitarray_IMPLEMENT_ME
#define ybitarray_IMPLEMENT_ME
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
YBITARRAY_EXPORT yt_func_exit_code_t
ybitarray_init (
        struct _ybitarray_t * ybitarray,
        const char * app_name);

//! terminate
///
YBITARRAY_EXPORT void
ybitarray_end (
        struct _ybitarray_t * ybitarray);

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
