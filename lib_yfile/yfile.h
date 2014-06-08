/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yfile.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_yfile_h_INCLUDE
#define YT_yfile_h_INCLUDE
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

struct _yfile_t;

//! a logger structure
///
typedef struct _yfile_t {

    const char * app_name_;     /**< the reported name of the application */

} yfile_t;


//! may be used to add special directives to export function
/// @{
#ifndef YFILE_EXPORT
#define YFILE_EXPORT
#endif
/// @}


//! not yet implemented
/// @{
#ifndef YFILE_IMPLEMENT_ME
#define YFILE_IMPLEMENT_ME
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
YFILE_EXPORT yt_func_exit_code_t
yfile_init (
        struct _yfile_t * yfile,
        const char * app_name);

//! terminate
///
YFILE_EXPORT void
yfile_end (
        struct _yfile_t * yfile);

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
#endif // YT_yfile_h_INCLUDE
