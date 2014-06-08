/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ylogger.c
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

#include "ylogger.h"

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
    if (ylogger_implicit_instance == NULL) { \
        ylogger_new (&ylogger_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ylogger_t * ylogger_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ylogger_default_err (ylogger_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylogger_default_wrn (ylogger_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylogger_default_info (ylogger_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylogger_default_dbg (ylogger_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylogger_init_implicit (
        const char * app_name)
{
    return ylogger_new (&ylogger_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylogger_init (ylogger_t * ylogger, const char * app_name)
{
    yt_func_start;

    memset (ylogger, 0, sizeof(ylogger_t));

    // by default enable all types
    ylogger->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ylogger->err_ = ylogger_default_err;
    ylogger->wrn_ = ylogger_default_wrn;
    ylogger->info_ = ylogger_default_info;
    ylogger->dbg_ = ylogger_default_dbg;

    // copy app name for our use
    yt_func_null(ylogger->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ylogger->syslog_ = 1;
    openlog (ylogger->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ylogger->callbacks_ = 0;
#else
    ylogger->syslog_ = 0;

    // enable logging through callbacks
    ylogger->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylogger_end (ylogger_t * ylogger)
{
    memset (ylogger, 0, sizeof(ylogger_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylogger_new (ylogger_t ** ylogger, const char * app_name)
{
    DBG_ASSERT(ylogger != NULL);\
    ylogger_t * ylogger_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ylogger_loc, (ylogger_t*)malloc (sizeof(ylogger_t)));

    // and prepare it
    exitcode = ylogger_init (ylogger_loc, app_name);

    yt_func_end;

    *ylogger = ylogger_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylogger_free (ylogger_t ** ylogger)
{
    DBG_ASSERT(ylogger != NULL);\
    ylogger_t * ylogger_loc = *ylogger;
    if (ylogger_loc != NULL) {
        ylogger_end (ylogger_loc);
        free (ylogger_loc);
        *ylogger = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ylogger_t *
ylogger_implicit ()
{
    enshure_implicit;
    return ylogger_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ylogger_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ylogger != NULL);\
    if (!ylogger_is_enabled (ylogger, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ylogger->callbacks_) && (ylogger->__kb__ != NULL)) {\
        ylogger->__kb__(ylogger, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ylogger_common_syslog \
    if (ylogger->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ylogger_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylogger_err ( ylogger_t * ylogger, const char *format, ...)
{
    ylogger_common_init(YLOGGER_ERROR, err_);
    ylogger_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylogger_wrn ( ylogger_t * ylogger, const char *format, ...)
{
    ylogger_common_init(YLOGGER_WARNING, wrn_);
    ylogger_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylogger_info ( ylogger_t * ylogger, const char *format, ...)
{
    ylogger_common_init(YLOGGER_INFO, info_);
    ylogger_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ylogger_dbg ( ylogger_t * ylogger, const char *format, ...)
{
    ylogger_common_init(YLOGGER_DEBUG, dbg_);
    ylogger_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ylogger ylogger_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ylogger_common_init(YLOGGER_ERROR, err_);
    ylogger_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ylogger_common_init(YLOGGER_WARNING, wrn_);
    ylogger_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ylogger_common_init(YLOGGER_INFO, info_);
    ylogger_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ylogger_common_init(YLOGGER_DEBUG, dbg_);
    ylogger_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ylogger

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
