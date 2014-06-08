/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ymarginsf.c
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

#include "ymarginsf.h"

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
    if (ymarginsf_implicit_instance == NULL) { \
        ymarginsf_new (&ymarginsf_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ymarginsf_t * ymarginsf_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ymarginsf_default_err (ymarginsf_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymarginsf_default_wrn (ymarginsf_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymarginsf_default_info (ymarginsf_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ymarginsf_default_dbg (ymarginsf_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymarginsf_init_implicit (
        const char * app_name)
{
    return ymarginsf_new (&ymarginsf_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymarginsf_init (ymarginsf_t * ymarginsf, const char * app_name)
{
    yt_func_start;

    memset (ymarginsf, 0, sizeof(ymarginsf_t));

    // by default enable all types
    ymarginsf->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ymarginsf->err_ = ymarginsf_default_err;
    ymarginsf->wrn_ = ymarginsf_default_wrn;
    ymarginsf->info_ = ymarginsf_default_info;
    ymarginsf->dbg_ = ymarginsf_default_dbg;

    // copy app name for our use
    yt_func_null(ymarginsf->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ymarginsf->syslog_ = 1;
    openlog (ymarginsf->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ymarginsf->callbacks_ = 0;
#else
    ymarginsf->syslog_ = 0;

    // enable logging through callbacks
    ymarginsf->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymarginsf_end (ymarginsf_t * ymarginsf)
{
    memset (ymarginsf, 0, sizeof(ymarginsf_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ymarginsf_new (ymarginsf_t ** ymarginsf, const char * app_name)
{
    DBG_ASSERT(ymarginsf != NULL);\
    ymarginsf_t * ymarginsf_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ymarginsf_loc, (ymarginsf_t*)malloc (sizeof(ymarginsf_t)));

    // and prepare it
    exitcode = ymarginsf_init (ymarginsf_loc, app_name);

    yt_func_end;

    *ymarginsf = ymarginsf_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymarginsf_free (ymarginsf_t ** ymarginsf)
{
    DBG_ASSERT(ymarginsf != NULL);\
    ymarginsf_t * ymarginsf_loc = *ymarginsf;
    if (ymarginsf_loc != NULL) {
        ymarginsf_end (ymarginsf_loc);
        free (ymarginsf_loc);
        *ymarginsf = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ymarginsf_t *
ymarginsf_implicit ()
{
    enshure_implicit;
    return ymarginsf_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ymarginsf_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ymarginsf != NULL);\
    if (!ymarginsf_is_enabled (ymarginsf, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ymarginsf->callbacks_) && (ymarginsf->__kb__ != NULL)) {\
        ymarginsf->__kb__(ymarginsf, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ymarginsf_common_syslog \
    if (ymarginsf->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ymarginsf_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymarginsf_err ( ymarginsf_t * ymarginsf, const char *format, ...)
{
    ymarginsf_common_init(YLOGGER_ERROR, err_);
    ymarginsf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymarginsf_wrn ( ymarginsf_t * ymarginsf, const char *format, ...)
{
    ymarginsf_common_init(YLOGGER_WARNING, wrn_);
    ymarginsf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ymarginsf_info ( ymarginsf_t * ymarginsf, const char *format, ...)
{
    ymarginsf_common_init(YLOGGER_INFO, info_);
    ymarginsf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ymarginsf_dbg ( ymarginsf_t * ymarginsf, const char *format, ...)
{
    ymarginsf_common_init(YLOGGER_DEBUG, dbg_);
    ymarginsf_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ymarginsf ymarginsf_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ymarginsf_common_init(YLOGGER_ERROR, err_);
    ymarginsf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ymarginsf_common_init(YLOGGER_WARNING, wrn_);
    ymarginsf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ymarginsf_common_init(YLOGGER_INFO, info_);
    ymarginsf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ymarginsf_common_init(YLOGGER_DEBUG, dbg_);
    ymarginsf_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ymarginsf

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
