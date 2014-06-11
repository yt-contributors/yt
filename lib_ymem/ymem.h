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
#include <string.h>
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
#ifndef YMEM_EXPORT
#define YMEM_EXPORT
#endif
/// @}


//! malloc hook that returns a pointer to be returned to the user
/// @{
#ifndef YMEM_MALLOC_HOOK
#define YMEM_MALLOC_HOOK(__size__)  malloc (__size__);
#endif
/// @}

//! free hook: a function that frees memory
/// @{
#ifndef YMEM_FREE_HOOK
#define YMEM_FREE_HOOK(__to_be_freed__)  free (__to_be_freed__)
#endif
/// @}

//! realloc hook: a function that changes the size
/// @{
#ifndef YMEM_REALLOC_HOOK
#define YMEM_REALLOC_HOOK(__ptr__, __size__)  realloc(__ptr__, __size__)
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
YMEM_EXPORT yt_func_exit_code_t
ymem_init (
        struct _ymem_t * ymem);

//! terminate
///
YMEM_EXPORT void
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
    void * ret = YMEM_MALLOC_HOOK(size);
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
    void * ret = YMEM_REALLOC_HOOK (ptr, size);
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
    YMEM_FREE_HOOK(ptr);
}

//! create a copy of a utf8 null terminated string
///
static inline char *
ymem_strdup (
        const char * ptr)
{
    char * ret = strdup (ptr);
    if (ret == NULL) err_message ("Memory allocation error in ymem_strdup");
    return ret;
}

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Fast stack allocation
 *  If the space is enough then stack buffer is used,
 *  otherwise a dynamic one is allocated.
 *
 *  Example:
 *  @code
 *      YT_STACKBUFF_INIT(buff, char, 256, sz_p + 1 + sz_n + 1);
 *      // ...
 *      YT_STACKBUFF_END(buff)
 *  @endcode
 *  This will define following code:
 *  @code
 *  char buff_array[256];
 *  char * buff_ptr = buff_array;
 *  int buff_is_dynamic = 0;
 *  size_t buff_actual_sz = sz_p + 1 + sz_n + 1;
 *  if (buff_actual_sz > 256) {
 *    buff_ptr = (char*)ymem_malloc(sizeof(char)*buff_actual_sz);
 *    buff_is_dynamic = 0;
 *  }
 *  // ...
 *  if (buff_is_dynamic != 0) {
 *    ymem_free ();
 *    buff_ptr = NULL;
 *  }
 *  @endcode
 */
///@{


//! allocate stack variable
///
#define YT_STACKBUFF_INIT(__varbase__, __type__, __stack_buf_sz__, __request_sz__) \
    __type__ __varbase__ ## _array[__stack_buf_sz__]; \
    __type__ * __varbase__ ## _ptr = __varbase__ ## _array; \
    int __varbase__ ## _is_dynamic = 0; \
    size_t __varbase__ ## _actual_sz = (__request_sz__); \
    if ( (__varbase__ ## _actual_sz) > (__stack_buf_sz__) ) { \
        __varbase__ ## _ptr = (__type__*)ymem_malloc (\
                sizeof(__type__) * (__varbase__ ## _actual_sz)); \
        __varbase__ ## _is_dynamic = 1; \
    }

//! free stack variable
///
#define YT_STACKBUFF_END(__varbase__) \
    if ( __varbase__ ## _is_dynamic != 0 ) {\
        ymem_free (__varbase__ ## _ptr); \
        __varbase__ ## _ptr = NULL; \
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
