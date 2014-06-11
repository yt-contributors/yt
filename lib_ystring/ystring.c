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

//! at debug time we check for mysterious values
#ifndef YSTRING_APPEND_MAX_DEBUG
#define YSTRING_APPEND_MAX_DEBUG 64
#endif


static inline void ystring_release_buffer_simple (ystring_t * ystring)
{
    ycntbuff_dec ((void*)&ystring->buffer_, ystring);
    ystring->bytes_alloc_ = 0;
    ystring->bytes_used_ = 0;
    ystring->flags_ &= (~YSTRING_COPY_ON_WRITE);
}

static inline int ystring_is_shared (ystring_t * ystring)
{
    DBG_ASSERT(ystring != NULL);
    return ((ystring->flags_ & YSTRING_COPY_ON_WRITE) != 0);
}

static inline int ystring_alloc_size (size_t actual_len)
{
    DBG_ASSERT(actual_len > 0);

    size_t allocated_len;
    if (actual_len < YSTRING_MIN_BUFF_SIZE) {
        allocated_len = YSTRING_MIN_BUFF_SIZE;
    } else if (actual_len < YSTRING_MED_BUFF_SIZE) {
        allocated_len = actual_len*2;
    } else {
        allocated_len = actual_len+1;
    }
    return allocated_len;
}

static void ystring_share (
        ystring_t * ystring_from, ystring_t * ystring_to)
{
    // copy the characteristics of the foreign buffer locally
    ystring_to->bytes_alloc_ = ystring_from->bytes_alloc_;
    ystring_to->bytes_used_ = ystring_from->bytes_used_;

    // increment the reference count of source buffer
    DBG_ASSERT(ycntbuff_count(ystring_from->buffer_) > 0);
    ycntbuff_inc ((void*)ystring_from->buffer_, ystring_to);
    ystring_to->buffer_ = ystring_from->buffer_;

    // both instances become copy-on-write until reference count reaches 1
    ystring_from->flags_ |= YSTRING_COPY_ON_WRITE;
    ystring_to->flags_ = ystring_from->flags_;
}

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
    size_t allocated_len = ystring_alloc_size (actual_len);

    // allocate this much and copy; save sizes
    yt_func_ok (ycntbuff_new (
                    (void**)&ystring->buffer_,
                    ystring, allocated_len));
    memcpy ((void*)ystring->buffer_, value, actual_len);
    ystring->bytes_alloc_ = allocated_len;
    ystring->bytes_used_ = actual_len;

    yt_func_end;
    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT yt_func_exit_code_t
ystring_init_counted (
        ystring_t * ystring, const char * value, size_t value_len)

{
    yt_func_start;

    // clear the structure to 0
    memset (ystring, 0, sizeof(ystring_t));
    if (value == NULL) break;
    if (value_len == 0) break;

    // see how long is the string (in bytes) and decide a buffer size
    size_t actual_len = value_len+1;
    size_t allocated_len = ystring_alloc_size (actual_len);

    // allocate this much and copy; save sizes
    yt_func_ok (ycntbuff_new (
                    (void**)&ystring->buffer_,
                    ystring, allocated_len));

    // copy it and null-terminate
    char * tmp = (char*)ystring->buffer_;
    memcpy (tmp, value, value_len);
    tmp[value_len] = 0;

    ystring->bytes_alloc_ = allocated_len;
    ystring->bytes_used_ = actual_len;

    yt_func_end;
    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT yt_func_exit_code_t
ystring_init_c (ystring_t * ystring, ystring_t * ystring_source)
{
    if (ystring == NULL) return YT_FUNC_BAD_INPUT;

    yt_func_start;

    if ((ystring_source == NULL) || ystring_is_empty (ystring_source)) {
        // for cases where is no benefit in duplicating the source
        exitcode = ystring_init (ystring, NULL);
    } else {
        // clear the structure to 0's and get a shared copy
        memset (ystring, 0, sizeof(ystring_t));
        ystring_share (ystring_source, ystring);
    }

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
        ycntbuff_dec ((void**)&ystring->buffer_, ystring);
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
    DBG_ASSERT_RET(ystring != NULL);
    ystring_t * ret = *ystring;
    if (ret == NULL) {
        ystring_end (ret);
    }
    ymem_free (ret);

    *ystring = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT void
ystring_clear (ystring_t * ystring)
{
    DBG_ASSERT_RET(ystring != NULL);
    if (ystring->buffer_ == NULL) return;
    if (ystring_is_shared (ystring)) {
        ystring_release_buffer_simple (ystring);
    } else if (ystring->bytes_alloc_ > YSTRING_MED_BUFF_SIZE) {
        ystring_release_buffer_simple (ystring);
    } else {
        ystring->bytes_used_ = 0;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT yt_func_exit_code_t
ystring_set (ystring_t * ystring, const char * value)
{
    if (ystring == NULL) return YT_FUNC_BAD_INPUT;
    yt_func_start;
    if (value == NULL) {
        // no input so we want a clear string
        ystring_clear (ystring);
    } else if (ystring->buffer_ == NULL) {
        // we have no buffer so we're pristine
        exitcode = ystring_init (ystring, value);
    } else if (ystring_is_shared (ystring)) {
        // give up our reference and reinit
        ystring_release_buffer_simple (ystring);
        exitcode = ystring_init (ystring, value);
    } else {
        // buffer is ours; see if is large enough
        size_t actual_len = strlen (value)+1;
        if (ystring->bytes_alloc_ > actual_len) {
            // not large enough; create a new one
            void * temp_buff = NULL;
            size_t allocated_len = ystring_alloc_size (actual_len);
            yt_func_ok (ycntbuff_new (
                            &temp_buff,
                            ystring, allocated_len));
            // release our current reference
            ycntbuff_dec ((void*)&ystring->buffer_, ystring);

            // save new state
            ystring->buffer_ = (const char*)temp_buff;
            ystring->bytes_used_ = 0;
            ystring->bytes_alloc_ = allocated_len;
        }
        memcpy ((void*)ystring->buffer_, value, actual_len);
        ystring->bytes_used_ = actual_len;
    }

    yt_func_end;
    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT yt_func_exit_code_t
ystring_copy (ystring_t * ystring_from, ystring_t * ystring_to)
{
    if (ystring_from == NULL) return YT_FUNC_BAD_INPUT;
    if (ystring_to == NULL) return YT_FUNC_BAD_INPUT;

    yt_func_start;

    // discard any content we may have
    if (ystring_to->buffer_ != NULL) {
        ystring_release_buffer_simple (ystring_to);
    }
    // and get a shared copy
    if (!ystring_is_empty (ystring_from)) {
        ystring_share (ystring_from, ystring_to);
    }
    yt_func_end;
    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT yt_func_exit_code_t
ystring_append (ystring_t * ystring_result, size_t count, ...)
{
    if (ystring_result == NULL) return YT_FUNC_BAD_INPUT;
#   ifdef YT_DEBUG
    if (count > YSTRING_APPEND_MAX_DEBUG) {YT_BREAKPOINT;}
#   endif // YT_DEBUG

    char * result_buffer = NULL;
    char * result_iter = NULL;
    va_list argptr;
    size_t result_len = 1;
    ystring_t * ys_iter;
    size_t i;
    size_t intermed_sz;

    yt_func_start;

    // add first one
    if (!ystring_is_empty (ystring_result)) {
        result_len += ystring_result->bytes_used_ - 1;
    }

    // add all the rest
    va_start (argptr, count);
    for (i = 0; i < count; ++i) {
        ys_iter = va_arg(argptr, ystring_t*);
        if (!ystring_is_empty (ys_iter)) {
            result_len += ys_iter->bytes_used_ - 1;
        }
    }
    va_end (argptr);

    // we have a sum; allocate this many bytes
    size_t allocated_len = ystring_alloc_size (result_len);
    yt_func_ok (ycntbuff_new (
                    (void**)&result_buffer,
                    ystring_result, allocated_len));
    result_iter = result_buffer;

    // add first one
    if (!ystring_is_empty (ystring_result)) {
        intermed_sz = ystring_result->bytes_used_ - 1;
        memcpy ((void*)result_iter, ystring_result->buffer_, intermed_sz);
        result_iter += intermed_sz;
    }

    // add all the rest
    va_start (argptr, count);
    for (i = 0; i < count; ++i) {
        ys_iter = va_arg(argptr, ystring_t*);
        if (!ystring_is_empty (ys_iter)) {
            intermed_sz = ys_iter->bytes_used_ - 1;
            memcpy ((void*)result_iter, ys_iter->buffer_, intermed_sz);
            result_iter += intermed_sz;
        }
    }
    va_end (argptr);

    // make it NULL-terminated
    result_iter[0] = 0;

    // get rid of our buffer and set up the new one
    ystring_release_buffer_simple (ystring_result);
    ystring_result->buffer_ = result_buffer;
    ystring_result->bytes_alloc_ = allocated_len;
    ystring_result->bytes_used_ = result_len;
    result_buffer = NULL;

    yt_func_end;
    if (result_buffer != NULL) {
        ycntbuff_dec ((void**)&result_buffer, ystring_result);
    }
    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YSTRING_EXPORT yt_func_exit_code_t
ystring_reserve (ystring_t * ys, size_t new_len)
{
    yt_func_start;
    char * result_buffer = NULL;
    size_t allocated_len = ystring_alloc_size (new_len);
    if (ystring_is_null (ys)) {
        yt_func_ok (ycntbuff_new (
                        (void**)&result_buffer,
                        ys, new_len));
        ys->buffer_ = result_buffer;
        ys->bytes_alloc_ = allocated_len;
        break;
    } else if (!ystring_is_shared (ys)) {
        if (ys->bytes_alloc_ > allocated_len) break;
    }
    yt_func_ok (ycntbuff_new (
                    (void**)&result_buffer,
                    ys, allocated_len));
    memcpy (result_buffer, ys->buffer_, ys->bytes_used_);
    ycntbuff_dec ((void**)&ys->buffer_, ys);
    ys->buffer_ = result_buffer;
    ys->bytes_alloc_ = allocated_len;
    ys->flags_ &= (~YSTRING_COPY_ON_WRITE);
    yt_func_end;
    yt_func_ret;
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
