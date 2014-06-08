/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yvariant.c
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

#include "yvariant.h"

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
    if (yvariant_implicit_instance == NULL) { \
        yvariant_new (&yvariant_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static yvariant_t * yvariant_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void yvariant_default_err (yvariant_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yvariant_default_wrn (yvariant_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yvariant_default_info (yvariant_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yvariant_default_dbg (yvariant_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yvariant_init_implicit (
        const char * app_name)
{
    return yvariant_new (&yvariant_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yvariant_init (yvariant_t * yvariant, const char * app_name)
{
    yt_func_start;

    memset (yvariant, 0, sizeof(yvariant_t));

    // by default enable all types
    yvariant->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    yvariant->err_ = yvariant_default_err;
    yvariant->wrn_ = yvariant_default_wrn;
    yvariant->info_ = yvariant_default_info;
    yvariant->dbg_ = yvariant_default_dbg;

    // copy app name for our use
    yt_func_null(yvariant->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    yvariant->syslog_ = 1;
    openlog (yvariant->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    yvariant->callbacks_ = 0;
#else
    yvariant->syslog_ = 0;

    // enable logging through callbacks
    yvariant->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yvariant_end (yvariant_t * yvariant)
{
    memset (yvariant, 0, sizeof(yvariant_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yvariant_new (yvariant_t ** yvariant, const char * app_name)
{
    DBG_ASSERT(yvariant != NULL);\
    yvariant_t * yvariant_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(yvariant_loc, (yvariant_t*)malloc (sizeof(yvariant_t)));

    // and prepare it
    exitcode = yvariant_init (yvariant_loc, app_name);

    yt_func_end;

    *yvariant = yvariant_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yvariant_free (yvariant_t ** yvariant)
{
    DBG_ASSERT(yvariant != NULL);\
    yvariant_t * yvariant_loc = *yvariant;
    if (yvariant_loc != NULL) {
        yvariant_end (yvariant_loc);
        free (yvariant_loc);
        *yvariant = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yvariant_t *
yvariant_implicit ()
{
    enshure_implicit;
    return yvariant_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define yvariant_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(yvariant != NULL);\
    if (!yvariant_is_enabled (yvariant, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((yvariant->callbacks_) && (yvariant->__kb__ != NULL)) {\
        yvariant->__kb__(yvariant, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define yvariant_common_syslog \
    if (yvariant->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define yvariant_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yvariant_err ( yvariant_t * yvariant, const char *format, ...)
{
    yvariant_common_init(YLOGGER_ERROR, err_);
    yvariant_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yvariant_wrn ( yvariant_t * yvariant, const char *format, ...)
{
    yvariant_common_init(YLOGGER_WARNING, wrn_);
    yvariant_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yvariant_info ( yvariant_t * yvariant, const char *format, ...)
{
    yvariant_common_init(YLOGGER_INFO, info_);
    yvariant_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
yvariant_dbg ( yvariant_t * yvariant, const char *format, ...)
{
    yvariant_common_init(YLOGGER_DEBUG, dbg_);
    yvariant_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define yvariant yvariant_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    yvariant_common_init(YLOGGER_ERROR, err_);
    yvariant_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    yvariant_common_init(YLOGGER_WARNING, wrn_);
    yvariant_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    yvariant_common_init(YLOGGER_INFO, info_);
    yvariant_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    yvariant_common_init(YLOGGER_DEBUG, dbg_);
    yvariant_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef yvariant

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
