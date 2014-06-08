/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ysizef.c
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

#include "ysizef.h"

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
    if (ysizef_implicit_instance == NULL) { \
        ysizef_new (&ysizef_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ysizef_t * ysizef_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ysizef_default_err (ysizef_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ysizef_default_wrn (ysizef_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ysizef_default_info (ysizef_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ysizef_default_dbg (ysizef_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ysizef_init_implicit (
        const char * app_name)
{
    return ysizef_new (&ysizef_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ysizef_init (ysizef_t * ysizef, const char * app_name)
{
    yt_func_start;

    memset (ysizef, 0, sizeof(ysizef_t));

    // by default enable all types
    ysizef->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ysizef->err_ = ysizef_default_err;
    ysizef->wrn_ = ysizef_default_wrn;
    ysizef->info_ = ysizef_default_info;
    ysizef->dbg_ = ysizef_default_dbg;

    // copy app name for our use
    yt_func_null(ysizef->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ysizef->syslog_ = 1;
    openlog (ysizef->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ysizef->callbacks_ = 0;
#else
    ysizef->syslog_ = 0;

    // enable logging through callbacks
    ysizef->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ysizef_end (ysizef_t * ysizef)
{
    memset (ysizef, 0, sizeof(ysizef_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ysizef_new (ysizef_t ** ysizef, const char * app_name)
{
    DBG_ASSERT(ysizef != NULL);\
    ysizef_t * ysizef_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ysizef_loc, (ysizef_t*)malloc (sizeof(ysizef_t)));

    // and prepare it
    exitcode = ysizef_init (ysizef_loc, app_name);

    yt_func_end;

    *ysizef = ysizef_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ysizef_free (ysizef_t ** ysizef)
{
    DBG_ASSERT(ysizef != NULL);\
    ysizef_t * ysizef_loc = *ysizef;
    if (ysizef_loc != NULL) {
        ysizef_end (ysizef_loc);
        free (ysizef_loc);
        *ysizef = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ysizef_t *
ysizef_implicit ()
{
    enshure_implicit;
    return ysizef_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ysizef_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ysizef != NULL);\
    if (!ysizef_is_enabled (ysizef, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ysizef->callbacks_) && (ysizef->__kb__ != NULL)) {\
        ysizef->__kb__(ysizef, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ysizef_common_syslog \
    if (ysizef->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ysizef_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ysizef_err ( ysizef_t * ysizef, const char *format, ...)
{
    ysizef_common_init(YLOGGER_ERROR, err_);
    ysizef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ysizef_wrn ( ysizef_t * ysizef, const char *format, ...)
{
    ysizef_common_init(YLOGGER_WARNING, wrn_);
    ysizef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ysizef_info ( ysizef_t * ysizef, const char *format, ...)
{
    ysizef_common_init(YLOGGER_INFO, info_);
    ysizef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ysizef_dbg ( ysizef_t * ysizef, const char *format, ...)
{
    ysizef_common_init(YLOGGER_DEBUG, dbg_);
    ysizef_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ysizef ysizef_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ysizef_common_init(YLOGGER_ERROR, err_);
    ysizef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ysizef_common_init(YLOGGER_WARNING, wrn_);
    ysizef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ysizef_common_init(YLOGGER_INFO, info_);
    ysizef_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ysizef_common_init(YLOGGER_DEBUG, dbg_);
    ysizef_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ysizef

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
