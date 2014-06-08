/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ylist.c
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

#include "ylist.h"

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
    if (ylist_implicit_instance == NULL) { \
        ylist_new (&ylist_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ylist_t * ylist_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ylist_default_err (ylist_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylist_default_wrn (ylist_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylist_default_info (ylist_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylist_default_dbg (ylist_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylist_init_implicit (
        const char * app_name)
{
    return ylist_new (&ylist_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylist_init (ylist_t * ylist, const char * app_name)
{
    yt_func_start;

    memset (ylist, 0, sizeof(ylist_t));

    // by default enable all types
    ylist->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ylist->err_ = ylist_default_err;
    ylist->wrn_ = ylist_default_wrn;
    ylist->info_ = ylist_default_info;
    ylist->dbg_ = ylist_default_dbg;

    // copy app name for our use
    yt_func_null(ylist->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ylist->syslog_ = 1;
    openlog (ylist->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ylist->callbacks_ = 0;
#else
    ylist->syslog_ = 0;

    // enable logging through callbacks
    ylist->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylist_end (ylist_t * ylist)
{
    memset (ylist, 0, sizeof(ylist_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylist_new (ylist_t ** ylist, const char * app_name)
{
    DBG_ASSERT(ylist != NULL);\
    ylist_t * ylist_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ylist_loc, (ylist_t*)malloc (sizeof(ylist_t)));

    // and prepare it
    exitcode = ylist_init (ylist_loc, app_name);

    yt_func_end;

    *ylist = ylist_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylist_free (ylist_t ** ylist)
{
    DBG_ASSERT(ylist != NULL);\
    ylist_t * ylist_loc = *ylist;
    if (ylist_loc != NULL) {
        ylist_end (ylist_loc);
        free (ylist_loc);
        *ylist = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ylist_t *
ylist_implicit ()
{
    enshure_implicit;
    return ylist_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ylist_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ylist != NULL);\
    if (!ylist_is_enabled (ylist, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ylist->callbacks_) && (ylist->__kb__ != NULL)) {\
        ylist->__kb__(ylist, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ylist_common_syslog \
    if (ylist->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ylist_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylist_err ( ylist_t * ylist, const char *format, ...)
{
    ylist_common_init(YLOGGER_ERROR, err_);
    ylist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylist_wrn ( ylist_t * ylist, const char *format, ...)
{
    ylist_common_init(YLOGGER_WARNING, wrn_);
    ylist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylist_info ( ylist_t * ylist, const char *format, ...)
{
    ylist_common_init(YLOGGER_INFO, info_);
    ylist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ylist_dbg ( ylist_t * ylist, const char *format, ...)
{
    ylist_common_init(YLOGGER_DEBUG, dbg_);
    ylist_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ylist ylist_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ylist_common_init(YLOGGER_ERROR, err_);
    ylist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ylist_common_init(YLOGGER_WARNING, wrn_);
    ylist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ylist_common_init(YLOGGER_INFO, info_);
    ylist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ylist_common_init(YLOGGER_DEBUG, dbg_);
    ylist_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ylist

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
