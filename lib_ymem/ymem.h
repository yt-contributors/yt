/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ymem.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ymem_h_INCLUDE
#define YT_ymem_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <yt/yt.h>
#include <stdlib.h>
#include <yt/ylogger.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


//! a logger structure
///
typedef struct _ymem_t {

} ymem_t;


//! may be used to add special directives to export function
/// @{
#ifndef YLOGGER_EXPORT
#define YLOGGER_EXPORT
#endif
/// @}


//! not yet implemented
/// @{
#ifndef YLOGGER_IMPLEMENT_ME
#define YLOGGER_IMPLEMENT_ME
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
YLOGGER_EXPORT yt_func_exit_code_t
ymem_init (
        struct _ymem_t * ymem);

//! terminate
///
YLOGGER_EXPORT void
ymem_end (
        struct _ymem_t * ymem);

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Normal, everyday allocation
 *  Drop-in replacement for malloc, realloc and free
 */
///@{

//! allocate
///
/// General purpose memory allocator
///
static inline void*
ymem_malloc (
        size_t size)
{
    void * ret = malloc (size);
    if (ret == NULL) err_message ("Memory allocation error in ymem_malloc");
    return ret;
}

//! change the size of a block of allocated memory
///
/// General purpose memory allocator
///
static inline void*
ymem_realloc (
        void * ptr,
        size_t size)
{
    void * ret = realloc (ptr, size);
    if (ret == NULL) err_message ("Memory allocation error in ymem_realloc");
    return ret;
}

//! release a block of allocated memory using ymem_malloc()
///
/// Free memory.
///
static inline void
ymem_free (
        void * ptr)
{
    free (ptr);
}

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
#endif // YT_ymem_h_INCLUDE
