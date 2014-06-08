/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yrect.c
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

#include "yrect.h"

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
    if (yrect_implicit_instance == NULL) { \
        yrect_new (&yrect_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static yrect_t * yrect_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void yrect_default_err (yrect_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yrect_default_wrn (yrect_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yrect_default_info (yrect_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yrect_default_dbg (yrect_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yrect_init_implicit (
        const char * app_name)
{
    return yrect_new (&yrect_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yrect_init (yrect_t * yrect, const char * app_name)
{
    yt_func_start;

    memset (yrect, 0, sizeof(yrect_t));

    // by default enable all types
    yrect->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    yrect->err_ = yrect_default_err;
    yrect->wrn_ = yrect_default_wrn;
    yrect->info_ = yrect_default_info;
    yrect->dbg_ = yrect_default_dbg;

    // copy app name for our use
    yt_func_null(yrect->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    yrect->syslog_ = 1;
    openlog (yrect->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    yrect->callbacks_ = 0;
#else
    yrect->syslog_ = 0;

    // enable logging through callbacks
    yrect->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yrect_end (yrect_t * yrect)
{
    memset (yrect, 0, sizeof(yrect_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yrect_new (yrect_t ** yrect, const char * app_name)
{
    DBG_ASSERT(yrect != NULL);\
    yrect_t * yrect_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(yrect_loc, (yrect_t*)malloc (sizeof(yrect_t)));

    // and prepare it
    exitcode = yrect_init (yrect_loc, app_name);

    yt_func_end;

    *yrect = yrect_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yrect_free (yrect_t ** yrect)
{
    DBG_ASSERT(yrect != NULL);\
    yrect_t * yrect_loc = *yrect;
    if (yrect_loc != NULL) {
        yrect_end (yrect_loc);
        free (yrect_loc);
        *yrect = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yrect_t *
yrect_implicit ()
{
    enshure_implicit;
    return yrect_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define yrect_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(yrect != NULL);\
    if (!yrect_is_enabled (yrect, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((yrect->callbacks_) && (yrect->__kb__ != NULL)) {\
        yrect->__kb__(yrect, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define yrect_common_syslog \
    if (yrect->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define yrect_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yrect_err ( yrect_t * yrect, const char *format, ...)
{
    yrect_common_init(YLOGGER_ERROR, err_);
    yrect_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yrect_wrn ( yrect_t * yrect, const char *format, ...)
{
    yrect_common_init(YLOGGER_WARNING, wrn_);
    yrect_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yrect_info ( yrect_t * yrect, const char *format, ...)
{
    yrect_common_init(YLOGGER_INFO, info_);
    yrect_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
yrect_dbg ( yrect_t * yrect, const char *format, ...)
{
    yrect_common_init(YLOGGER_DEBUG, dbg_);
    yrect_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define yrect yrect_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    yrect_common_init(YLOGGER_ERROR, err_);
    yrect_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    yrect_common_init(YLOGGER_WARNING, wrn_);
    yrect_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    yrect_common_init(YLOGGER_INFO, info_);
    yrect_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    yrect_common_init(YLOGGER_DEBUG, dbg_);
    yrect_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef yrect

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
