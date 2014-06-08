/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ysize.c
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

#include "ysize.h"

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

#define default_reporting(__insert__) \
 \
time_t curtime = time (NULL); \
struct tm *loctime = localtime (&curtime); \
char formatted[64]; \
strftime (formatted, 64, "%y-%m-%d %H:%M:%S " __insert__ " ", loctime); \
fprintf (stderr, "%s", formatted); \
 \
vfprintf (stderr, format, arg); \
fprintf (stderr, "\n"); \

#define enshure_implicit \
    if (ysize_implicit_instance == NULL) { \
        ysize_new (&ysize_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ysize_t * ysize_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ysize_default_err (ysize_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ysize_default_wrn (ysize_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ysize_default_info (ysize_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ysize_default_dbg (ysize_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ysize_init_implicit (
        const char * app_name)
{
    return ysize_new (&ysize_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ysize_init (ysize_t * ysize, const char * app_name)
{
    yt_func_start;

    memset (ysize, 0, sizeof(ysize_t));

    // by default enable all types
    ysize->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ysize->err_ = ysize_default_err;
    ysize->wrn_ = ysize_default_wrn;
    ysize->info_ = ysize_default_info;
    ysize->dbg_ = ysize_default_dbg;

    // copy app name for our use
    yt_func_null(ysize->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ysize->syslog_ = 1;
    openlog (ysize->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ysize->callbacks_ = 0;
#else
    ysize->syslog_ = 0;

    // enable logging through callbacks
    ysize->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ysize_end (ysize_t * ysize)
{
    memset (ysize, 0, sizeof(ysize_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ysize_new (ysize_t ** ysize, const char * app_name)
{
    DBG_ASSERT(ysize != NULL);\
    ysize_t * ysize_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ysize_loc, (ysize_t*)malloc (sizeof(ysize_t)));

    // and prepare it
    exitcode = ysize_init (ysize_loc, app_name);

    yt_func_end;

    *ysize = ysize_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ysize_free (ysize_t ** ysize)
{
    DBG_ASSERT(ysize != NULL);\
    ysize_t * ysize_loc = *ysize;
    if (ysize_loc != NULL) {
        ysize_end (ysize_loc);
        free (ysize_loc);
        *ysize = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ysize_t *
ysize_implicit ()
{
    enshure_implicit;
    return ysize_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ysize_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ysize != NULL);\
    if (!ysize_is_enabled (ysize, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ysize->callbacks_) && (ysize->__kb__ != NULL)) {\
        ysize->__kb__(ysize, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ysize_common_syslog \
    if (ysize->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ysize_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ysize_err ( ysize_t * ysize, const char *format, ...)
{
    ysize_common_init(YLOGGER_ERROR, err_);
    ysize_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ysize_wrn ( ysize_t * ysize, const char *format, ...)
{
    ysize_common_init(YLOGGER_WARNING, wrn_);
    ysize_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ysize_info ( ysize_t * ysize, const char *format, ...)
{
    ysize_common_init(YLOGGER_INFO, info_);
    ysize_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ysize_dbg ( ysize_t * ysize, const char *format, ...)
{
    ysize_common_init(YLOGGER_DEBUG, dbg_);
    ysize_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ysize ysize_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ysize_common_init(YLOGGER_ERROR, err_);
    ysize_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ysize_common_init(YLOGGER_WARNING, wrn_);
    ysize_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ysize_common_init(YLOGGER_INFO, info_);
    ysize_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ysize_common_init(YLOGGER_DEBUG, dbg_);
    ysize_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ysize

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
