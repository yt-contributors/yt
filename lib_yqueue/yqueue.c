/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yqueue.c
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

#include "yqueue.h"

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
    if (yqueue_implicit_instance == NULL) { \
        yqueue_new (&yqueue_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static yqueue_t * yqueue_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void yqueue_default_err (yqueue_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yqueue_default_wrn (yqueue_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yqueue_default_info (yqueue_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yqueue_default_dbg (yqueue_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yqueue_init_implicit (
        const char * app_name)
{
    return yqueue_new (&yqueue_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yqueue_init (yqueue_t * yqueue, const char * app_name)
{
    yt_func_start;

    memset (yqueue, 0, sizeof(yqueue_t));

    // by default enable all types
    yqueue->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    yqueue->err_ = yqueue_default_err;
    yqueue->wrn_ = yqueue_default_wrn;
    yqueue->info_ = yqueue_default_info;
    yqueue->dbg_ = yqueue_default_dbg;

    // copy app name for our use
    yt_func_null(yqueue->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    yqueue->syslog_ = 1;
    openlog (yqueue->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    yqueue->callbacks_ = 0;
#else
    yqueue->syslog_ = 0;

    // enable logging through callbacks
    yqueue->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yqueue_end (yqueue_t * yqueue)
{
    memset (yqueue, 0, sizeof(yqueue_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yqueue_new (yqueue_t ** yqueue, const char * app_name)
{
    DBG_ASSERT(yqueue != NULL);\
    yqueue_t * yqueue_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(yqueue_loc, (yqueue_t*)malloc (sizeof(yqueue_t)));

    // and prepare it
    exitcode = yqueue_init (yqueue_loc, app_name);

    yt_func_end;

    *yqueue = yqueue_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yqueue_free (yqueue_t ** yqueue)
{
    DBG_ASSERT(yqueue != NULL);\
    yqueue_t * yqueue_loc = *yqueue;
    if (yqueue_loc != NULL) {
        yqueue_end (yqueue_loc);
        free (yqueue_loc);
        *yqueue = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yqueue_t *
yqueue_implicit ()
{
    enshure_implicit;
    return yqueue_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define yqueue_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(yqueue != NULL);\
    if (!yqueue_is_enabled (yqueue, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((yqueue->callbacks_) && (yqueue->__kb__ != NULL)) {\
        yqueue->__kb__(yqueue, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define yqueue_common_syslog \
    if (yqueue->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define yqueue_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yqueue_err ( yqueue_t * yqueue, const char *format, ...)
{
    yqueue_common_init(YLOGGER_ERROR, err_);
    yqueue_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yqueue_wrn ( yqueue_t * yqueue, const char *format, ...)
{
    yqueue_common_init(YLOGGER_WARNING, wrn_);
    yqueue_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yqueue_info ( yqueue_t * yqueue, const char *format, ...)
{
    yqueue_common_init(YLOGGER_INFO, info_);
    yqueue_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
yqueue_dbg ( yqueue_t * yqueue, const char *format, ...)
{
    yqueue_common_init(YLOGGER_DEBUG, dbg_);
    yqueue_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define yqueue yqueue_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    yqueue_common_init(YLOGGER_ERROR, err_);
    yqueue_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    yqueue_common_init(YLOGGER_WARNING, wrn_);
    yqueue_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    yqueue_common_init(YLOGGER_INFO, info_);
    yqueue_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    yqueue_common_init(YLOGGER_DEBUG, dbg_);
    yqueue_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef yqueue

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
