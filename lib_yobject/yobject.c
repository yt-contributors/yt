/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yobject.c
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

#include "yobject.h"

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
    if (yobject_implicit_instance == NULL) { \
        yobject_new (&yobject_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static yobject_t * yobject_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void yobject_default_err (yobject_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yobject_default_wrn (yobject_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yobject_default_info (yobject_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yobject_default_dbg (yobject_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yobject_init_implicit (
        const char * app_name)
{
    return yobject_new (&yobject_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yobject_init (yobject_t * yobject, const char * app_name)
{
    yt_func_start;

    memset (yobject, 0, sizeof(yobject_t));

    // by default enable all types
    yobject->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    yobject->err_ = yobject_default_err;
    yobject->wrn_ = yobject_default_wrn;
    yobject->info_ = yobject_default_info;
    yobject->dbg_ = yobject_default_dbg;

    // copy app name for our use
    yt_func_null(yobject->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    yobject->syslog_ = 1;
    openlog (yobject->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    yobject->callbacks_ = 0;
#else
    yobject->syslog_ = 0;

    // enable logging through callbacks
    yobject->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yobject_end (yobject_t * yobject)
{
    memset (yobject, 0, sizeof(yobject_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yobject_new (yobject_t ** yobject, const char * app_name)
{
    DBG_ASSERT(yobject != NULL);\
    yobject_t * yobject_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(yobject_loc, (yobject_t*)malloc (sizeof(yobject_t)));

    // and prepare it
    exitcode = yobject_init (yobject_loc, app_name);

    yt_func_end;

    *yobject = yobject_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yobject_free (yobject_t ** yobject)
{
    DBG_ASSERT(yobject != NULL);\
    yobject_t * yobject_loc = *yobject;
    if (yobject_loc != NULL) {
        yobject_end (yobject_loc);
        free (yobject_loc);
        *yobject = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yobject_t *
yobject_implicit ()
{
    enshure_implicit;
    return yobject_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define yobject_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(yobject != NULL);\
    if (!yobject_is_enabled (yobject, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((yobject->callbacks_) && (yobject->__kb__ != NULL)) {\
        yobject->__kb__(yobject, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define yobject_common_syslog \
    if (yobject->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define yobject_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yobject_err ( yobject_t * yobject, const char *format, ...)
{
    yobject_common_init(YLOGGER_ERROR, err_);
    yobject_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yobject_wrn ( yobject_t * yobject, const char *format, ...)
{
    yobject_common_init(YLOGGER_WARNING, wrn_);
    yobject_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yobject_info ( yobject_t * yobject, const char *format, ...)
{
    yobject_common_init(YLOGGER_INFO, info_);
    yobject_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
yobject_dbg ( yobject_t * yobject, const char *format, ...)
{
    yobject_common_init(YLOGGER_DEBUG, dbg_);
    yobject_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define yobject yobject_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    yobject_common_init(YLOGGER_ERROR, err_);
    yobject_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    yobject_common_init(YLOGGER_WARNING, wrn_);
    yobject_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    yobject_common_init(YLOGGER_INFO, info_);
    yobject_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    yobject_common_init(YLOGGER_DEBUG, dbg_);
    yobject_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef yobject

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
