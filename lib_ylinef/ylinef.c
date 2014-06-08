/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ylinef.c
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

#include "ylinef.h"

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
    if (ylinef_implicit_instance == NULL) { \
        ylinef_new (&ylinef_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ylinef_t * ylinef_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ylinef_default_err (ylinef_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylinef_default_wrn (ylinef_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylinef_default_info (ylinef_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylinef_default_dbg (ylinef_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylinef_init_implicit (
        const char * app_name)
{
    return ylinef_new (&ylinef_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylinef_init (ylinef_t * ylinef, const char * app_name)
{
    yt_func_start;

    memset (ylinef, 0, sizeof(ylinef_t));

    // by default enable all types
    ylinef->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ylinef->err_ = ylinef_default_err;
    ylinef->wrn_ = ylinef_default_wrn;
    ylinef->info_ = ylinef_default_info;
    ylinef->dbg_ = ylinef_default_dbg;

    // copy app name for our use
    yt_func_null(ylinef->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ylinef->syslog_ = 1;
    openlog (ylinef->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ylinef->callbacks_ = 0;
#else
    ylinef->syslog_ = 0;

    // enable logging through callbacks
    ylinef->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylinef_end (ylinef_t * ylinef)
{
    memset (ylinef, 0, sizeof(ylinef_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylinef_new (ylinef_t ** ylinef, const char * app_name)
{
    DBG_ASSERT(ylinef != NULL);\
    ylinef_t * ylinef_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ylinef_loc, (ylinef_t*)malloc (sizeof(ylinef_t)));

    // and prepare it
    exitcode = ylinef_init (ylinef_loc, app_name);

    yt_func_end;

    *ylinef = ylinef_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylinef_free (ylinef_t ** ylinef)
{
    DBG_ASSERT(ylinef != NULL);\
    ylinef_t * ylinef_loc = *ylinef;
    if (ylinef_loc != NULL) {
        ylinef_end (ylinef_loc);
        free (ylinef_loc);
        *ylinef = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ylinef_t *
ylinef_implicit ()
{
    enshure_implicit;
    return ylinef_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ylinef_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ylinef != NULL);\
    if (!ylinef_is_enabled (ylinef, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ylinef->callbacks_) && (ylinef->__kb__ != NULL)) {\
        ylinef->__kb__(ylinef, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ylinef_common_syslog \
    if (ylinef->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ylinef_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylinef_err ( ylinef_t * ylinef, const char *format, ...)
{
    ylinef_common_init(YLOGGER_ERROR, err_);
    ylinef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylinef_wrn ( ylinef_t * ylinef, const char *format, ...)
{
    ylinef_common_init(YLOGGER_WARNING, wrn_);
    ylinef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylinef_info ( ylinef_t * ylinef, const char *format, ...)
{
    ylinef_common_init(YLOGGER_INFO, info_);
    ylinef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ylinef_dbg ( ylinef_t * ylinef, const char *format, ...)
{
    ylinef_common_init(YLOGGER_DEBUG, dbg_);
    ylinef_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ylinef ylinef_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ylinef_common_init(YLOGGER_ERROR, err_);
    ylinef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ylinef_common_init(YLOGGER_WARNING, wrn_);
    ylinef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ylinef_common_init(YLOGGER_INFO, info_);
    ylinef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ylinef_common_init(YLOGGER_DEBUG, dbg_);
    ylinef_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ylinef

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
