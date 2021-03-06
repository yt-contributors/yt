/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ystack.c
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

#include "ystack.h"

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
    if (ystack_implicit_instance == NULL) { \
        ystack_new (&ystack_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ystack_t * ystack_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ystack_default_err (ystack_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ystack_default_wrn (ystack_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ystack_default_info (ystack_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ystack_default_dbg (ystack_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ystack_init_implicit (
        const char * app_name)
{
    return ystack_new (&ystack_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ystack_init (ystack_t * ystack, const char * app_name)
{
    yt_func_start;

    memset (ystack, 0, sizeof(ystack_t));

    // by default enable all types
    ystack->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ystack->err_ = ystack_default_err;
    ystack->wrn_ = ystack_default_wrn;
    ystack->info_ = ystack_default_info;
    ystack->dbg_ = ystack_default_dbg;

    // copy app name for our use
    yt_func_null(ystack->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ystack->syslog_ = 1;
    openlog (ystack->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ystack->callbacks_ = 0;
#else
    ystack->syslog_ = 0;

    // enable logging through callbacks
    ystack->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ystack_end (ystack_t * ystack)
{
    memset (ystack, 0, sizeof(ystack_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ystack_new (ystack_t ** ystack, const char * app_name)
{
    DBG_ASSERT(ystack != NULL);\
    ystack_t * ystack_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ystack_loc, (ystack_t*)malloc (sizeof(ystack_t)));

    // and prepare it
    exitcode = ystack_init (ystack_loc, app_name);

    yt_func_end;

    *ystack = ystack_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ystack_free (ystack_t ** ystack)
{
    DBG_ASSERT(ystack != NULL);\
    ystack_t * ystack_loc = *ystack;
    if (ystack_loc != NULL) {
        ystack_end (ystack_loc);
        free (ystack_loc);
        *ystack = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ystack_t *
ystack_implicit ()
{
    enshure_implicit;
    return ystack_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ystack_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ystack != NULL);\
    if (!ystack_is_enabled (ystack, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ystack->callbacks_) && (ystack->__kb__ != NULL)) {\
        ystack->__kb__(ystack, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ystack_common_syslog \
    if (ystack->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ystack_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ystack_err ( ystack_t * ystack, const char *format, ...)
{
    ystack_common_init(YLOGGER_ERROR, err_);
    ystack_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ystack_wrn ( ystack_t * ystack, const char *format, ...)
{
    ystack_common_init(YLOGGER_WARNING, wrn_);
    ystack_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ystack_info ( ystack_t * ystack, const char *format, ...)
{
    ystack_common_init(YLOGGER_INFO, info_);
    ystack_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ystack_dbg ( ystack_t * ystack, const char *format, ...)
{
    ystack_common_init(YLOGGER_DEBUG, dbg_);
    ystack_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ystack ystack_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ystack_common_init(YLOGGER_ERROR, err_);
    ystack_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ystack_common_init(YLOGGER_WARNING, wrn_);
    ystack_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ystack_common_init(YLOGGER_INFO, info_);
    ystack_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ystack_common_init(YLOGGER_DEBUG, dbg_);
    ystack_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ystack

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
