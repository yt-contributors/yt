/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ymap.c
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

#include "ymap.h"

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
    if (ymap_implicit_instance == NULL) { \
        ymap_new (&ymap_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ymap_t * ymap_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ymap_default_err (ymap_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymap_default_wrn (ymap_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymap_default_info (ymap_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymap_default_dbg (ymap_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymap_init_implicit (
        const char * app_name)
{
    return ymap_new (&ymap_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymap_init (ymap_t * ymap, const char * app_name)
{
    yt_func_start;

    memset (ymap, 0, sizeof(ymap_t));

    // by default enable all types
    ymap->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ymap->err_ = ymap_default_err;
    ymap->wrn_ = ymap_default_wrn;
    ymap->info_ = ymap_default_info;
    ymap->dbg_ = ymap_default_dbg;

    // copy app name for our use
    yt_func_null(ymap->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ymap->syslog_ = 1;
    openlog (ymap->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ymap->callbacks_ = 0;
#else
    ymap->syslog_ = 0;

    // enable logging through callbacks
    ymap->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymap_end (ymap_t * ymap)
{
    memset (ymap, 0, sizeof(ymap_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymap_new (ymap_t ** ymap, const char * app_name)
{
    DBG_ASSERT(ymap != NULL);\
    ymap_t * ymap_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ymap_loc, (ymap_t*)malloc (sizeof(ymap_t)));

    // and prepare it
    exitcode = ymap_init (ymap_loc, app_name);

    yt_func_end;

    *ymap = ymap_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymap_free (ymap_t ** ymap)
{
    DBG_ASSERT(ymap != NULL);\
    ymap_t * ymap_loc = *ymap;
    if (ymap_loc != NULL) {
        ymap_end (ymap_loc);
        free (ymap_loc);
        *ymap = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ymap_t *
ymap_implicit ()
{
    enshure_implicit;
    return ymap_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ymap_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ymap != NULL);\
    if (!ymap_is_enabled (ymap, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ymap->callbacks_) && (ymap->__kb__ != NULL)) {\
        ymap->__kb__(ymap, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ymap_common_syslog \
    if (ymap->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ymap_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymap_err ( ymap_t * ymap, const char *format, ...)
{
    ymap_common_init(YLOGGER_ERROR, err_);
    ymap_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymap_wrn ( ymap_t * ymap, const char *format, ...)
{
    ymap_common_init(YLOGGER_WARNING, wrn_);
    ymap_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymap_info ( ymap_t * ymap, const char *format, ...)
{
    ymap_common_init(YLOGGER_INFO, info_);
    ymap_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ymap_dbg ( ymap_t * ymap, const char *format, ...)
{
    ymap_common_init(YLOGGER_DEBUG, dbg_);
    ymap_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ymap ymap_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ymap_common_init(YLOGGER_ERROR, err_);
    ymap_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ymap_common_init(YLOGGER_WARNING, wrn_);
    ymap_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ymap_common_init(YLOGGER_INFO, info_);
    ymap_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ymap_common_init(YLOGGER_DEBUG, dbg_);
    ymap_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ymap

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
