/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ybitarray.c
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

#include "ybitarray.h"

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
YBITARRAY_EXPORT yt_func_exit_code_t
ybitarray_init_bytes (
        ybitarray_t * ybitarray, const void * source, size_t bytes_count)
{
    yt_func_start;
    // allocate
    size_t actual_bytes_count = bytes_count / 8;
    if (actual_bytes_count * 8 != bytes_count) ++actual_bytes_count;
    yt_func_ok (ystring_init_empty (ybitarray));
    yt_func_ok (ystring_reserve (ybitarray, actual_bytes_count));
    // fill
    size_t i;
    const char * iter_source = (const char *)source;
    char * iter_dest = (char *)ybitarray->buffer_;
    size_t bit_pos = 0;
    char crt_val = 0;
    for (i = 0; i < bytes_count; ++i) {
        if (*iter_source != 0)
            crt_val = crt_val | (1 << bit_pos++);
        if (bit_pos == 8) {
            *iter_dest = crt_val;
            crt_val = 0;
            bit_pos = 0;
            ++iter_source;
        }
    }
    if (bit_pos != 0) {
        *iter_dest = crt_val;
        crt_val = 0;
        bit_pos = 0;
    }
    yt_func_end;
    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YBITARRAY_EXPORT yt_func_exit_code_t
ybitarray_set (ybitarray_t * ybitarray, size_t offset, int value)
{
    if (ybitarray == NULL) return YT_FUNC_BAD_INPUT;
    size_t byte_offset = offset / 8;
    size_t bit_shift = offset - (byte_offset * 8);
    if (byte_offset > ybitarray->bytes_used_) return YT_FUNC_BAD_INPUT;
    char byte_value = ybitarray->buffer_[byte_offset];
    char * p = (char*)ybitarray->buffer_;
    if (value) {
        byte_value |= 1 << bit_shift;
    } else {
        byte_value &= ~(1 << bit_shift);
    }
    p[byte_offset] = byte_value;
    return YT_FUNC_OK;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YBITARRAY_EXPORT int
ybitarray_get (ybitarray_t * ybitarray, size_t offset)
{
    if (ybitarray == NULL) return 0;
    size_t byte_offset = offset / 8;
    size_t bit_shift = offset - (byte_offset * 8);
    if (byte_offset > ybitarray->bytes_used_) return 0;
    char byte_value = ybitarray->buffer_[byte_offset];
    return (byte_value & (1 << bit_shift)) != 0;
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
