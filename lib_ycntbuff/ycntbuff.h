/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ycntbuff.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ycntbuff_h_INCLUDE
#define YT_ycntbuff_h_INCLUDE
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

//! a reference-counted buffer
///
/// This structure acts as a header for the buffer. The memory chunk
/// that is returned is immediately following this structure.
///
typedef struct _ycntbuff_t {
    size_t count_;
#ifdef YT_DEBUG
    void ** owners_list_;
    size_t allocated_sz_;
    size_t used_sz_;
    size_t freed_sz_;
#endif
} ycntbuff_t;


//! may be used to add special directives to export function
/// @{
#ifndef YCNTBUFF_EXPORT
#   define YCNTBUFF_EXPORT
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

//! create a new reference-counted buffer
///
/// The buffer is created with a reference count of 1 that is
/// associated with \b owner. NULL is not a valid owner.
///
/// On debug build the owners are stored in a list; increments add
/// owners to the list while decrements remove owners from the list.
/// This is usefull for debugging purposes. On release builds the
/// owner is completly disregarded in all calls and it does not
/// increase the runtime cost.
///
YCNTBUFF_EXPORT yt_func_exit_code_t
ycntbuff_new (
        void ** buffer_holder,
        const void * owner,
        size_t usable_size);

//! Decrements the reference counter
///
/// If the counter has reached 0 the buffer is freed.
/// In any case location pointed by\b buffer_holder
/// will contain NULL.  NULL is not a valid owner.
///
/// In debug builds, if the owner was not found inside the list,
/// an error will be shown and execution stopped by a debug break
/// point.
///
YCNTBUFF_EXPORT void
ycntbuff_dec (
        void ** buffer_holder,
        const void * owner);

//! Increments the reference counter
///
/// In debug builds the owner is added to internal list.
/// NULL is not a valid owner.
///
YCNTBUFF_EXPORT void
ycntbuff_inc (
        void * buffer,
        const void * owner);

//! Get the reference counter
///
YCNTBUFF_EXPORT size_t
ycntbuff_count (
        const void * buffer);

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
#endif // YT_ycntbuff_h_INCLUDE
