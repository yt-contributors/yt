/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ystring.c
  \date			June 2014
  \author		Nicu Tofan

  \brief


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "ystring.h"
#include <yt/ymem.h>
#include <yt/ycntbuff.h>

#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifdef YT_SYSLOG_FOUND
#   include "syslog.h"
#endif // SYSLOG_FOUND

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

//! the buffer for any given string does not get below this size
#define YSTRING_MIN_BUFF_SIZE   32

//! above this size we stick to the size of the string
#define YSTRING_MED_BUFF_SIZE   256



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

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT yt_func_exit_code_t
ystring_init (ystring_t * ystring, const char * value)
{
    yt_func_start;

    // clear the structure to 0
    memset (ystring, 0, sizeof(ystring_t));
    if (value == NULL) break;

    // see how long is the string (in bytes) and decide a buffer size
    size_t actual_len = strlen (value)+1;
    size_t allocated_len;
    if (actual_len < YSTRING_MIN_BUFF_SIZE) {
        allocated_len = YSTRING_MIN_BUFF_SIZE;
    } else if (actual_len < YSTRING_MED_BUFF_SIZE) {
        allocated_len = actual_len*2;
    } else {
        allocated_len = actual_len+1;
    }

    // allocate this much and copy; save sizes
    yt_func_ok (ycntbuff_new (
                    &ystring->buffer_,
                    ystring, allocated_len));
    memcpy ((void*)ystring->buffer_, value, actual_len);
    ystring->bytes_alloc_ = allocated_len;
    ystring->bytes_used_ = actual_len;

    yt_func_end;
    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT void
ystring_end (ystring_t * ystring)
{
    // release the buffer
    if (ystring->buffer_ != NULL) {
        ycntbuff_dec(&ystring->buffer_, ystring);
    }

    // clear the structure
    memset (ystring, 0, sizeof(ystring_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT yt_func_exit_code_t
ystring_new (ystring_t ** ystring, const char * value)
{
    if (ystring == NULL) return YT_FUNC_BAD_INPUT;

    ystring_t * ret = NULL;
    yt_func_start;
    yt_func_null(ret, ymem_malloc (sizeof(ystring_t)));
    yt_func_ok( ystring_init (ret, value));
    yt_func_end;
    *ystring = ret;
    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT void
ystring_free (ystring_t ** ystring)
{
    DBG_ASSERT(ystring != NULL);

    ystring_t * ret = *ystring;
    if (ret == NULL) {
        ystring_end (ret);
    }
    ymem_free (ret);

    *ystring = NULL;
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
