/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ypointf.c
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

#include "ypointf.h"

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
    if (ypointf_implicit_instance == NULL) { \
        ypointf_new (&ypointf_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ypointf_t * ypointf_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ypointf_default_err (ypointf_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ypointf_default_wrn (ypointf_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ypointf_default_info (ypointf_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ypointf_default_dbg (ypointf_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ypointf_init_implicit (
        const char * app_name)
{
    return ypointf_new (&ypointf_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ypointf_init (ypointf_t * ypointf, const char * app_name)
{
    yt_func_start;

    memset (ypointf, 0, sizeof(ypointf_t));

    // by default enable all types
    ypointf->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ypointf->err_ = ypointf_default_err;
    ypointf->wrn_ = ypointf_default_wrn;
    ypointf->info_ = ypointf_default_info;
    ypointf->dbg_ = ypointf_default_dbg;

    // copy app name for our use
    yt_func_null(ypointf->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ypointf->syslog_ = 1;
    openlog (ypointf->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ypointf->callbacks_ = 0;
#else
    ypointf->syslog_ = 0;

    // enable logging through callbacks
    ypointf->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ypointf_end (ypointf_t * ypointf)
{
    memset (ypointf, 0, sizeof(ypointf_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ypointf_new (ypointf_t ** ypointf, const char * app_name)
{
    DBG_ASSERT(ypointf != NULL);\
    ypointf_t * ypointf_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ypointf_loc, (ypointf_t*)malloc (sizeof(ypointf_t)));

    // and prepare it
    exitcode = ypointf_init (ypointf_loc, app_name);

    yt_func_end;

    *ypointf = ypointf_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ypointf_free (ypointf_t ** ypointf)
{
    DBG_ASSERT(ypointf != NULL);\
    ypointf_t * ypointf_loc = *ypointf;
    if (ypointf_loc != NULL) {
        ypointf_end (ypointf_loc);
        free (ypointf_loc);
        *ypointf = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ypointf_t *
ypointf_implicit ()
{
    enshure_implicit;
    return ypointf_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ypointf_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ypointf != NULL);\
    if (!ypointf_is_enabled (ypointf, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ypointf->callbacks_) && (ypointf->__kb__ != NULL)) {\
        ypointf->__kb__(ypointf, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ypointf_common_syslog \
    if (ypointf->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ypointf_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ypointf_err ( ypointf_t * ypointf, const char *format, ...)
{
    ypointf_common_init(YLOGGER_ERROR, err_);
    ypointf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ypointf_wrn ( ypointf_t * ypointf, const char *format, ...)
{
    ypointf_common_init(YLOGGER_WARNING, wrn_);
    ypointf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ypointf_info ( ypointf_t * ypointf, const char *format, ...)
{
    ypointf_common_init(YLOGGER_INFO, info_);
    ypointf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ypointf_dbg ( ypointf_t * ypointf, const char *format, ...)
{
    ypointf_common_init(YLOGGER_DEBUG, dbg_);
    ypointf_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ypointf ypointf_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ypointf_common_init(YLOGGER_ERROR, err_);
    ypointf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ypointf_common_init(YLOGGER_WARNING, wrn_);
    ypointf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ypointf_common_init(YLOGGER_INFO, info_);
    ypointf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ypointf_common_init(YLOGGER_DEBUG, dbg_);
    ypointf_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ypointf

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
