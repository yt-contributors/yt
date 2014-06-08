/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ylinkedlist.c
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

#include "ylinkedlist.h"

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
    if (ylinkedlist_implicit_instance == NULL) { \
        ylinkedlist_new (&ylinkedlist_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ylinkedlist_t * ylinkedlist_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ylinkedlist_default_err (ylinkedlist_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylinkedlist_default_wrn (ylinkedlist_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylinkedlist_default_info (ylinkedlist_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ylinkedlist_default_dbg (ylinkedlist_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylinkedlist_init_implicit (
        const char * app_name)
{
    return ylinkedlist_new (&ylinkedlist_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylinkedlist_init (ylinkedlist_t * ylinkedlist, const char * app_name)
{
    yt_func_start;

    memset (ylinkedlist, 0, sizeof(ylinkedlist_t));

    // by default enable all types
    ylinkedlist->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ylinkedlist->err_ = ylinkedlist_default_err;
    ylinkedlist->wrn_ = ylinkedlist_default_wrn;
    ylinkedlist->info_ = ylinkedlist_default_info;
    ylinkedlist->dbg_ = ylinkedlist_default_dbg;

    // copy app name for our use
    yt_func_null(ylinkedlist->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ylinkedlist->syslog_ = 1;
    openlog (ylinkedlist->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ylinkedlist->callbacks_ = 0;
#else
    ylinkedlist->syslog_ = 0;

    // enable logging through callbacks
    ylinkedlist->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylinkedlist_end (ylinkedlist_t * ylinkedlist)
{
    memset (ylinkedlist, 0, sizeof(ylinkedlist_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ylinkedlist_new (ylinkedlist_t ** ylinkedlist, const char * app_name)
{
    DBG_ASSERT(ylinkedlist != NULL);\
    ylinkedlist_t * ylinkedlist_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ylinkedlist_loc, (ylinkedlist_t*)malloc (sizeof(ylinkedlist_t)));

    // and prepare it
    exitcode = ylinkedlist_init (ylinkedlist_loc, app_name);

    yt_func_end;

    *ylinkedlist = ylinkedlist_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylinkedlist_free (ylinkedlist_t ** ylinkedlist)
{
    DBG_ASSERT(ylinkedlist != NULL);\
    ylinkedlist_t * ylinkedlist_loc = *ylinkedlist;
    if (ylinkedlist_loc != NULL) {
        ylinkedlist_end (ylinkedlist_loc);
        free (ylinkedlist_loc);
        *ylinkedlist = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ylinkedlist_t *
ylinkedlist_implicit ()
{
    enshure_implicit;
    return ylinkedlist_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ylinkedlist_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ylinkedlist != NULL);\
    if (!ylinkedlist_is_enabled (ylinkedlist, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ylinkedlist->callbacks_) && (ylinkedlist->__kb__ != NULL)) {\
        ylinkedlist->__kb__(ylinkedlist, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ylinkedlist_common_syslog \
    if (ylinkedlist->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ylinkedlist_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylinkedlist_err ( ylinkedlist_t * ylinkedlist, const char *format, ...)
{
    ylinkedlist_common_init(YLOGGER_ERROR, err_);
    ylinkedlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylinkedlist_wrn ( ylinkedlist_t * ylinkedlist, const char *format, ...)
{
    ylinkedlist_common_init(YLOGGER_WARNING, wrn_);
    ylinkedlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ylinkedlist_info ( ylinkedlist_t * ylinkedlist, const char *format, ...)
{
    ylinkedlist_common_init(YLOGGER_INFO, info_);
    ylinkedlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ylinkedlist_dbg ( ylinkedlist_t * ylinkedlist, const char *format, ...)
{
    ylinkedlist_common_init(YLOGGER_DEBUG, dbg_);
    ylinkedlist_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ylinkedlist ylinkedlist_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ylinkedlist_common_init(YLOGGER_ERROR, err_);
    ylinkedlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ylinkedlist_common_init(YLOGGER_WARNING, wrn_);
    ylinkedlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ylinkedlist_common_init(YLOGGER_INFO, info_);
    ylinkedlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ylinkedlist_common_init(YLOGGER_DEBUG, dbg_);
    ylinkedlist_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ylinkedlist

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
