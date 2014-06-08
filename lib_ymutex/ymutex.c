/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ymutex.c
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

#include "ymutex.h"

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
    if (ymutex_implicit_instance == NULL) { \
        ymutex_new (&ymutex_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ymutex_t * ymutex_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ymutex_default_err (ymutex_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymutex_default_wrn (ymutex_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymutex_default_info (ymutex_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymutex_default_dbg (ymutex_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymutex_init_implicit (
        const char * app_name)
{
    return ymutex_new (&ymutex_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymutex_init (ymutex_t * ymutex, const char * app_name)
{
    yt_func_start;

    memset (ymutex, 0, sizeof(ymutex_t));

    // by default enable all types
    ymutex->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ymutex->err_ = ymutex_default_err;
    ymutex->wrn_ = ymutex_default_wrn;
    ymutex->info_ = ymutex_default_info;
    ymutex->dbg_ = ymutex_default_dbg;

    // copy app name for our use
    yt_func_null(ymutex->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ymutex->syslog_ = 1;
    openlog (ymutex->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ymutex->callbacks_ = 0;
#else
    ymutex->syslog_ = 0;

    // enable logging through callbacks
    ymutex->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymutex_end (ymutex_t * ymutex)
{
    memset (ymutex, 0, sizeof(ymutex_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymutex_new (ymutex_t ** ymutex, const char * app_name)
{
    DBG_ASSERT(ymutex != NULL);\
    ymutex_t * ymutex_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ymutex_loc, (ymutex_t*)malloc (sizeof(ymutex_t)));

    // and prepare it
    exitcode = ymutex_init (ymutex_loc, app_name);

    yt_func_end;

    *ymutex = ymutex_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymutex_free (ymutex_t ** ymutex)
{
    DBG_ASSERT(ymutex != NULL);\
    ymutex_t * ymutex_loc = *ymutex;
    if (ymutex_loc != NULL) {
        ymutex_end (ymutex_loc);
        free (ymutex_loc);
        *ymutex = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ymutex_t *
ymutex_implicit ()
{
    enshure_implicit;
    return ymutex_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ymutex_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ymutex != NULL);\
    if (!ymutex_is_enabled (ymutex, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ymutex->callbacks_) && (ymutex->__kb__ != NULL)) {\
        ymutex->__kb__(ymutex, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ymutex_common_syslog \
    if (ymutex->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ymutex_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymutex_err ( ymutex_t * ymutex, const char *format, ...)
{
    ymutex_common_init(YLOGGER_ERROR, err_);
    ymutex_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymutex_wrn ( ymutex_t * ymutex, const char *format, ...)
{
    ymutex_common_init(YLOGGER_WARNING, wrn_);
    ymutex_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymutex_info ( ymutex_t * ymutex, const char *format, ...)
{
    ymutex_common_init(YLOGGER_INFO, info_);
    ymutex_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ymutex_dbg ( ymutex_t * ymutex, const char *format, ...)
{
    ymutex_common_init(YLOGGER_DEBUG, dbg_);
    ymutex_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ymutex ymutex_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ymutex_common_init(YLOGGER_ERROR, err_);
    ymutex_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ymutex_common_init(YLOGGER_WARNING, wrn_);
    ymutex_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ymutex_common_init(YLOGGER_INFO, info_);
    ymutex_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ymutex_common_init(YLOGGER_DEBUG, dbg_);
    ymutex_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ymutex

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
