/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yutf8.c
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

#include "yutf8.h"

#include <yt/ymem.h>


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
#ifdef TARGET_SYSTEM_WIN32
#   define HAVE_WIN_SNPRINTF
#else
#   ifndef HAVE_SNPRINTF
#       error "no vsnprintf replacement"
#   endif
#endif

/* ------------------------------------------------------------------------- */
YUTF8_EXPORT yt_func_exit_code_t
yutf8_init (yutf8_t * yutf8, const char * value)
{
    yt_func_start;
    yutf8->value_ = value;
    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YUTF8_EXPORT void
yutf8_end (yutf8_t * yutf8)
{
    yutf8->value_ = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YUTF8_EXPORT yt_func_exit_code_t
yutf8_join (yutf8_t * out, size_t count, ...)
{
    if (out == NULL) return YT_FUNC_BAD_INPUT;
    out->value_ = NULL;

    char * ret = NULL;
    char * ret_iter = NULL;

    yt_func_start;

    va_list argptr;
    size_t i;
    const char * crt_it;
    size_t result_len = 1;

    // compute the size of the output
    va_start (argptr, count);
    for (i = 0; i < count; ++i) {
        crt_it = va_arg(argptr, char*);
        result_len += (crt_it == NULL ? 0 : strlen(crt_it));
    }
    va_end (argptr);

    // allocate a buffer this big
    yt_func_null(ret, (char*)ymem_malloc (result_len));
    ret_iter = ret;

    // append each component
    size_t crt_len;
    va_start (argptr, count);
    for (i = 0; i < count; ++i) {
        crt_it = va_arg(argptr, char*);
        if (crt_it != NULL) {
            crt_len = strlen(crt_it);
            memcpy (ret_iter, crt_it, crt_len);
            ret_iter += crt_len;
        }
    }
    va_end (argptr);
    *ret_iter = 0;

    yt_func_end;
    out->value_ = ret;
    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YUTF8_EXPORT yt_func_exit_code_t
yutf8_join_sep (const char * separator, yutf8_t * out, size_t count, ...)
{
    if (out == NULL) return YT_FUNC_BAD_INPUT;
    out->value_ = NULL;

    char * ret = NULL;
    char * ret_iter = NULL;

    yt_func_start;

    // be consistent - create a zero-lenght string
    if (count == 0) {
        yt_func_null(ret, (char*)ymem_malloc (1));
        *ret = 0;
        break;
    }

    va_list argptr;
    size_t i;
    const char * crt_it;
    size_t result_len = 1;
    size_t sep_len = (separator == NULL) ? 0 : strlen (separator);

    // compute the size of the output
    va_start (argptr, count);
    for (i = 0; i < count; ++i) {
        crt_it = va_arg(argptr, char*);
        result_len += (crt_it == NULL ? 0 : strlen(crt_it)) + sep_len;
    }
    if (i > 1) result_len -= sep_len;
    va_end (argptr);

    // allocate a buffer this big
    yt_func_null(ret, (char*)ymem_malloc (result_len));
    ret_iter = ret;

    // append each component
    size_t crt_len;
    va_start (argptr, count);
    for (i = 0; i < count-1; ++i) {
        crt_it = va_arg(argptr, char*);
        if (crt_it != NULL) {
            crt_len = strlen(crt_it);
            memcpy (ret_iter, crt_it, crt_len);
            ret_iter += crt_len;
        }
        memcpy (ret_iter, separator, sep_len);
        ret_iter += sep_len;
    }
    crt_it = va_arg(argptr, char*);
    if (crt_it != NULL) {
        crt_len = strlen(crt_it);
        memcpy (ret_iter, crt_it, crt_len);
        ret_iter += crt_len;
    }
    va_end (argptr);
    *ret_iter = 0;

    yt_func_end;
    out->value_ = ret;
    yt_func_ret;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YUTF8_EXPORT yt_func_exit_code_t
yutf8_sprintf (yutf8_t * out, const char * format, ...)
{
    if (out == NULL) return YT_FUNC_BAD_INPUT;
    out->value_ = NULL;

    char * ret = NULL;
    va_list argptr;
    size_t result_len;

    yt_func_start;

    // be consistent - create a zero-lenght string
    if ((format == NULL) || (*format == 0)) {
        yt_func_null(ret, (char*)ymem_malloc (1));
        *ret = 0;
        break;
    }

    // compute the length
    va_start (argptr, format);
#if defined(HAVE_WIN_SNPRINTF)
    result_len = _vscprintf (format, argptr) + 1;
#elif defined(HAVE_SNPRINTF)
    char fake[16]; // don't just use 0
    result_len = vsnprintf (fake, 1, format, argptr) + 1;
#endif
    va_end (argptr);

    // allocate a buffer this big
    yt_func_null(ret, (char*)ymem_malloc (result_len));

    // do actual printing
    va_start (argptr, format);
#if defined(HAVE_WIN_SNPRINTF)
    vsprintf_s (ret, result_len, format, argptr);
#elif defined(HAVE_SNPRINTF)
    vsnprintf (ret, result_len, format, argptr);
#endif
    va_end (argptr);

    yt_func_end;
    out->value_ = ret;
    yt_func_ret;

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YUTF8_EXPORT yt_func_exit_code_t
yutf8_foreach (
        yutf8_t * yutf8, yutf8_foreach_kb kb, void * user)
{
    if (yutf8 == NULL) return YT_FUNC_BAD_INPUT;
    if (yutf8->value_ == NULL) return YT_FUNC_OK;

    const char * iter = yutf8->value_;
    uint32_t crt_letter;
    uint32_t crt_char;
    uint32_t second_char;
    uint32_t third_char;
    uint32_t fourth_char;
    size_t offset = 0;
    size_t index = 0;
    size_t addition;
    yt_func_exit_code_t exitcode = YT_FUNC_OK;

#   define BAD_SEQUENCE {YT_BREAKPOINT; break;}

    for (;;) {
        // get current character and advance the pointer
        crt_char = *iter++;
        if (crt_char == 0) break;

        if (crt_char & YUTF8_FIRST_BIT) {
            if (crt_char & YUTF8_THIRD_BIT) {
                if (crt_char & YUTF8_FOURTH_BIT) {
                    crt_letter  = (crt_char & 0x07) << 18;
                    second_char = *iter++;
                    if (second_char == 0) BAD_SEQUENCE
                    crt_letter += (second_char & 0x3f) << 12;
                    third_char  = *iter++;
                    if (third_char == 0) BAD_SEQUENCE
                    crt_letter += (third_char & 0x3f) << 6;;
                    fourth_char = *iter++;
                    if (fourth_char == 0) BAD_SEQUENCE
                    crt_letter += (fourth_char & 0x3f);
                    addition = 4;
                } else {
                    crt_letter  = (crt_char & 0x0f) << 12;
                    second_char = *iter++;
                    if (second_char == 0) BAD_SEQUENCE
                    crt_letter += (second_char & 0x3f) << 6;
                    third_char  = *iter++;
                    if (third_char == 0) BAD_SEQUENCE
                    crt_letter += (third_char & 0x3f);
                    addition = 3;
                }
            } else {
                crt_letter  = (crt_char & 0x1f) << 6;
                second_char = *iter++;
                if (second_char == 0) BAD_SEQUENCE;
                crt_letter += (second_char & 0x3f);
                addition = 2;
            }
        } else {
            crt_letter = crt_char;
            addition = 1;
        }

        // inform the user about this one
        exitcode = kb (yutf8, crt_letter, offset, index, user);
        if (exitcode != YT_FUNC_OK) break;

        // advance our counters
        offset += addition;
        ++index;
    }
    return exitcode;
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
