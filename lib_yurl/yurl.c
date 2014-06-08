/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yurl.c
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

#include "yurl.h"

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
    if (yurl_implicit_instance == NULL) { \
        yurl_new (&yurl_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static yurl_t * yurl_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void yurl_default_err (yurl_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yurl_default_wrn (yurl_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yurl_default_info (yurl_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yurl_default_dbg (yurl_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yurl_init_implicit (
        const char * app_name)
{
    return yurl_new (&yurl_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yurl_init (yurl_t * yurl, const char * app_name)
{
    yt_func_start;

    memset (yurl, 0, sizeof(yurl_t));

    // by default enable all types
    yurl->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    yurl->err_ = yurl_default_err;
    yurl->wrn_ = yurl_default_wrn;
    yurl->info_ = yurl_default_info;
    yurl->dbg_ = yurl_default_dbg;

    // copy app name for our use
    yt_func_null(yurl->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    yurl->syslog_ = 1;
    openlog (yurl->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    yurl->callbacks_ = 0;
#else
    yurl->syslog_ = 0;

    // enable logging through callbacks
    yurl->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yurl_end (yurl_t * yurl)
{
    memset (yurl, 0, sizeof(yurl_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yurl_new (yurl_t ** yurl, const char * app_name)
{
    DBG_ASSERT(yurl != NULL);\
    yurl_t * yurl_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(yurl_loc, (yurl_t*)malloc (sizeof(yurl_t)));

    // and prepare it
    exitcode = yurl_init (yurl_loc, app_name);

    yt_func_end;

    *yurl = yurl_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yurl_free (yurl_t ** yurl)
{
    DBG_ASSERT(yurl != NULL);\
    yurl_t * yurl_loc = *yurl;
    if (yurl_loc != NULL) {
        yurl_end (yurl_loc);
        free (yurl_loc);
        *yurl = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yurl_t *
yurl_implicit ()
{
    enshure_implicit;
    return yurl_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define yurl_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(yurl != NULL);\
    if (!yurl_is_enabled (yurl, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((yurl->callbacks_) && (yurl->__kb__ != NULL)) {\
        yurl->__kb__(yurl, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define yurl_common_syslog \
    if (yurl->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define yurl_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yurl_err ( yurl_t * yurl, const char *format, ...)
{
    yurl_common_init(YLOGGER_ERROR, err_);
    yurl_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yurl_wrn ( yurl_t * yurl, const char *format, ...)
{
    yurl_common_init(YLOGGER_WARNING, wrn_);
    yurl_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yurl_info ( yurl_t * yurl, const char *format, ...)
{
    yurl_common_init(YLOGGER_INFO, info_);
    yurl_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
yurl_dbg ( yurl_t * yurl, const char *format, ...)
{
    yurl_common_init(YLOGGER_DEBUG, dbg_);
    yurl_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define yurl yurl_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    yurl_common_init(YLOGGER_ERROR, err_);
    yurl_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    yurl_common_init(YLOGGER_WARNING, wrn_);
    yurl_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    yurl_common_init(YLOGGER_INFO, info_);
    yurl_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    yurl_common_init(YLOGGER_DEBUG, dbg_);
    yurl_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef yurl

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
