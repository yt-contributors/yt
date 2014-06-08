/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ymargins.c
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

#include "ymargins.h"

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
    if (ymargins_implicit_instance == NULL) { \
        ymargins_new (&ymargins_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ymargins_t * ymargins_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ymargins_default_err (ymargins_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymargins_default_wrn (ymargins_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymargins_default_info (ymargins_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymargins_default_dbg (ymargins_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymargins_init_implicit (
        const char * app_name)
{
    return ymargins_new (&ymargins_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymargins_init (ymargins_t * ymargins, const char * app_name)
{
    yt_func_start;

    memset (ymargins, 0, sizeof(ymargins_t));

    // by default enable all types
    ymargins->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ymargins->err_ = ymargins_default_err;
    ymargins->wrn_ = ymargins_default_wrn;
    ymargins->info_ = ymargins_default_info;
    ymargins->dbg_ = ymargins_default_dbg;

    // copy app name for our use
    yt_func_null(ymargins->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ymargins->syslog_ = 1;
    openlog (ymargins->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ymargins->callbacks_ = 0;
#else
    ymargins->syslog_ = 0;

    // enable logging through callbacks
    ymargins->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymargins_end (ymargins_t * ymargins)
{
    memset (ymargins, 0, sizeof(ymargins_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymargins_new (ymargins_t ** ymargins, const char * app_name)
{
    DBG_ASSERT(ymargins != NULL);\
    ymargins_t * ymargins_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ymargins_loc, (ymargins_t*)malloc (sizeof(ymargins_t)));

    // and prepare it
    exitcode = ymargins_init (ymargins_loc, app_name);

    yt_func_end;

    *ymargins = ymargins_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymargins_free (ymargins_t ** ymargins)
{
    DBG_ASSERT(ymargins != NULL);\
    ymargins_t * ymargins_loc = *ymargins;
    if (ymargins_loc != NULL) {
        ymargins_end (ymargins_loc);
        free (ymargins_loc);
        *ymargins = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ymargins_t *
ymargins_implicit ()
{
    enshure_implicit;
    return ymargins_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ymargins_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ymargins != NULL);\
    if (!ymargins_is_enabled (ymargins, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ymargins->callbacks_) && (ymargins->__kb__ != NULL)) {\
        ymargins->__kb__(ymargins, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ymargins_common_syslog \
    if (ymargins->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ymargins_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymargins_err ( ymargins_t * ymargins, const char *format, ...)
{
    ymargins_common_init(YLOGGER_ERROR, err_);
    ymargins_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymargins_wrn ( ymargins_t * ymargins, const char *format, ...)
{
    ymargins_common_init(YLOGGER_WARNING, wrn_);
    ymargins_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymargins_info ( ymargins_t * ymargins, const char *format, ...)
{
    ymargins_common_init(YLOGGER_INFO, info_);
    ymargins_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ymargins_dbg ( ymargins_t * ymargins, const char *format, ...)
{
    ymargins_common_init(YLOGGER_DEBUG, dbg_);
    ymargins_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ymargins ymargins_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ymargins_common_init(YLOGGER_ERROR, err_);
    ymargins_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ymargins_common_init(YLOGGER_WARNING, wrn_);
    ymargins_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ymargins_common_init(YLOGGER_INFO, info_);
    ymargins_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ymargins_common_init(YLOGGER_DEBUG, dbg_);
    ymargins_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ymargins

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
