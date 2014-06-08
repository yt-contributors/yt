/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yline.c
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

#include "yline.h"

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
    if (yline_implicit_instance == NULL) { \
        yline_new (&yline_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static yline_t * yline_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void yline_default_err (yline_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yline_default_wrn (yline_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yline_default_info (yline_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yline_default_dbg (yline_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yline_init_implicit (
        const char * app_name)
{
    return yline_new (&yline_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yline_init (yline_t * yline, const char * app_name)
{
    yt_func_start;

    memset (yline, 0, sizeof(yline_t));

    // by default enable all types
    yline->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    yline->err_ = yline_default_err;
    yline->wrn_ = yline_default_wrn;
    yline->info_ = yline_default_info;
    yline->dbg_ = yline_default_dbg;

    // copy app name for our use
    yt_func_null(yline->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    yline->syslog_ = 1;
    openlog (yline->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    yline->callbacks_ = 0;
#else
    yline->syslog_ = 0;

    // enable logging through callbacks
    yline->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yline_end (yline_t * yline)
{
    memset (yline, 0, sizeof(yline_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yline_new (yline_t ** yline, const char * app_name)
{
    DBG_ASSERT(yline != NULL);\
    yline_t * yline_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(yline_loc, (yline_t*)malloc (sizeof(yline_t)));

    // and prepare it
    exitcode = yline_init (yline_loc, app_name);

    yt_func_end;

    *yline = yline_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yline_free (yline_t ** yline)
{
    DBG_ASSERT(yline != NULL);\
    yline_t * yline_loc = *yline;
    if (yline_loc != NULL) {
        yline_end (yline_loc);
        free (yline_loc);
        *yline = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yline_t *
yline_implicit ()
{
    enshure_implicit;
    return yline_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define yline_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(yline != NULL);\
    if (!yline_is_enabled (yline, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((yline->callbacks_) && (yline->__kb__ != NULL)) {\
        yline->__kb__(yline, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define yline_common_syslog \
    if (yline->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define yline_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yline_err ( yline_t * yline, const char *format, ...)
{
    yline_common_init(YLOGGER_ERROR, err_);
    yline_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yline_wrn ( yline_t * yline, const char *format, ...)
{
    yline_common_init(YLOGGER_WARNING, wrn_);
    yline_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yline_info ( yline_t * yline, const char *format, ...)
{
    yline_common_init(YLOGGER_INFO, info_);
    yline_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
yline_dbg ( yline_t * yline, const char *format, ...)
{
    yline_common_init(YLOGGER_DEBUG, dbg_);
    yline_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define yline yline_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    yline_common_init(YLOGGER_ERROR, err_);
    yline_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    yline_common_init(YLOGGER_WARNING, wrn_);
    yline_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    yline_common_init(YLOGGER_INFO, info_);
    yline_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    yline_common_init(YLOGGER_DEBUG, dbg_);
    yline_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef yline

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
