/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yt.h
  \date			June 2014
  \author		TNick

*//*


 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Please read COPYING and README files in root folder
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef yt_h_INCLUDE
#define yt_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

// generated on the fly from config.h.in by CMake
#include <yt/config.h>

// function error codes
#include <yt/yt_func_exit_code.h>

// standard headers
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifndef STRINGIFY
#   define STRINGIFY(s) STRINGIFY_HELPER(s)
#   define STRINGIFY_HELPER(s) #s
#endif


#ifndef NULL
#   define NULL ((void*)0)
#endif


#ifndef VAR_UNUSED
#   define VAR_UNUSED(expr) do { (void)(expr); } while (0)
#endif


//! debug break point
#ifdef YT_DEBUG
#   define YT_BREAKPOINT  asm volatile ("int3;");
#else
#   define YT_BREAKPOINT
#endif


//! our assert
#ifdef YT_DEBUG
#  define DBG_ASSERT(a) \
    if (!(a)) {\
        printf("\nDBG_ASSERT Failed: " STRINGIFY(a) "\n"); \
        printf("%s[%d]: %s\n\n",__FILE__, __LINE__, __func__); \
        YT_BREAKPOINT; \
    }
#else
#  define DBG_ASSERT(a)
#endif


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
#endif // yt_h_INCLUDE
