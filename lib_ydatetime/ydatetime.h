/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ydatetime.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ydatetime_h_INCLUDE
#define YT_ydatetime_h_INCLUDE
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

struct _ydatetime_t;

//! a logger structure
///
typedef struct _ydatetime_t {

    const char * app_name_;     /**< the reported name of the application */

} ydatetime_t;


//! may be used to add special directives to export function
/// @{
#ifndef YDATETIME_EXPORT
#define YDATETIME_EXPORT
#endif
/// @}


//! not yet implemented
/// @{
#ifndef YDATETIME_IMPLEMENT_ME
#define YDATETIME_IMPLEMENT_ME
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
YDATETIME_EXPORT yt_func_exit_code_t
ydatetime_init (
        struct _ydatetime_t * ydatetime,
        const char * app_name);

//! terminate
///
YDATETIME_EXPORT void
ydatetime_end (
        struct _ydatetime_t * ydatetime);

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
#endif // YT_ydatetime_h_INCLUDE
