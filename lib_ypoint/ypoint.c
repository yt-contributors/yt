/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ypoint.c
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

#include "ypoint.h"

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
    if (ypoint_implicit_instance == NULL) { \
        ypoint_new (&ypoint_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ypoint_t * ypoint_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ypoint_default_err (ypoint_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ypoint_default_wrn (ypoint_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ypoint_default_info (ypoint_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ypoint_default_dbg (ypoint_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ypoint_init_implicit (
        const char * app_name)
{
    return ypoint_new (&ypoint_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ypoint_init (ypoint_t * ypoint, const char * app_name)
{
    yt_func_start;

    memset (ypoint, 0, sizeof(ypoint_t));

    // by default enable all types
    ypoint->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ypoint->err_ = ypoint_default_err;
    ypoint->wrn_ = ypoint_default_wrn;
    ypoint->info_ = ypoint_default_info;
    ypoint->dbg_ = ypoint_default_dbg;

    // copy app name for our use
    yt_func_null(ypoint->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ypoint->syslog_ = 1;
    openlog (ypoint->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ypoint->callbacks_ = 0;
#else
    ypoint->syslog_ = 0;

    // enable logging through callbacks
    ypoint->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ypoint_end (ypoint_t * ypoint)
{
    memset (ypoint, 0, sizeof(ypoint_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ypoint_new (ypoint_t ** ypoint, const char * app_name)
{
    DBG_ASSERT(ypoint != NULL);\
    ypoint_t * ypoint_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ypoint_loc, (ypoint_t*)malloc (sizeof(ypoint_t)));

    // and prepare it
    exitcode = ypoint_init (ypoint_loc, app_name);

    yt_func_end;

    *ypoint = ypoint_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ypoint_free (ypoint_t ** ypoint)
{
    DBG_ASSERT(ypoint != NULL);\
    ypoint_t * ypoint_loc = *ypoint;
    if (ypoint_loc != NULL) {
        ypoint_end (ypoint_loc);
        free (ypoint_loc);
        *ypoint = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ypoint_t *
ypoint_implicit ()
{
    enshure_implicit;
    return ypoint_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ypoint_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ypoint != NULL);\
    if (!ypoint_is_enabled (ypoint, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ypoint->callbacks_) && (ypoint->__kb__ != NULL)) {\
        ypoint->__kb__(ypoint, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ypoint_common_syslog \
    if (ypoint->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ypoint_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ypoint_err ( ypoint_t * ypoint, const char *format, ...)
{
    ypoint_common_init(YLOGGER_ERROR, err_);
    ypoint_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ypoint_wrn ( ypoint_t * ypoint, const char *format, ...)
{
    ypoint_common_init(YLOGGER_WARNING, wrn_);
    ypoint_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ypoint_info ( ypoint_t * ypoint, const char *format, ...)
{
    ypoint_common_init(YLOGGER_INFO, info_);
    ypoint_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ypoint_dbg ( ypoint_t * ypoint, const char *format, ...)
{
    ypoint_common_init(YLOGGER_DEBUG, dbg_);
    ypoint_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ypoint ypoint_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ypoint_common_init(YLOGGER_ERROR, err_);
    ypoint_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ypoint_common_init(YLOGGER_WARNING, wrn_);
    ypoint_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ypoint_common_init(YLOGGER_INFO, info_);
    ypoint_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ypoint_common_init(YLOGGER_DEBUG, dbg_);
    ypoint_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ypoint

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
