/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yrectf.c
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

#include "yrectf.h"

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
    if (yrectf_implicit_instance == NULL) { \
        yrectf_new (&yrectf_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static yrectf_t * yrectf_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void yrectf_default_err (yrectf_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yrectf_default_wrn (yrectf_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yrectf_default_info (yrectf_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yrectf_default_dbg (yrectf_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yrectf_init_implicit (
        const char * app_name)
{
    return yrectf_new (&yrectf_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yrectf_init (yrectf_t * yrectf, const char * app_name)
{
    yt_func_start;

    memset (yrectf, 0, sizeof(yrectf_t));

    // by default enable all types
    yrectf->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    yrectf->err_ = yrectf_default_err;
    yrectf->wrn_ = yrectf_default_wrn;
    yrectf->info_ = yrectf_default_info;
    yrectf->dbg_ = yrectf_default_dbg;

    // copy app name for our use
    yt_func_null(yrectf->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    yrectf->syslog_ = 1;
    openlog (yrectf->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    yrectf->callbacks_ = 0;
#else
    yrectf->syslog_ = 0;

    // enable logging through callbacks
    yrectf->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yrectf_end (yrectf_t * yrectf)
{
    memset (yrectf, 0, sizeof(yrectf_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yrectf_new (yrectf_t ** yrectf, const char * app_name)
{
    DBG_ASSERT(yrectf != NULL);\
    yrectf_t * yrectf_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(yrectf_loc, (yrectf_t*)malloc (sizeof(yrectf_t)));

    // and prepare it
    exitcode = yrectf_init (yrectf_loc, app_name);

    yt_func_end;

    *yrectf = yrectf_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yrectf_free (yrectf_t ** yrectf)
{
    DBG_ASSERT(yrectf != NULL);\
    yrectf_t * yrectf_loc = *yrectf;
    if (yrectf_loc != NULL) {
        yrectf_end (yrectf_loc);
        free (yrectf_loc);
        *yrectf = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yrectf_t *
yrectf_implicit ()
{
    enshure_implicit;
    return yrectf_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define yrectf_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(yrectf != NULL);\
    if (!yrectf_is_enabled (yrectf, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((yrectf->callbacks_) && (yrectf->__kb__ != NULL)) {\
        yrectf->__kb__(yrectf, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define yrectf_common_syslog \
    if (yrectf->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define yrectf_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yrectf_err ( yrectf_t * yrectf, const char *format, ...)
{
    yrectf_common_init(YLOGGER_ERROR, err_);
    yrectf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yrectf_wrn ( yrectf_t * yrectf, const char *format, ...)
{
    yrectf_common_init(YLOGGER_WARNING, wrn_);
    yrectf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yrectf_info ( yrectf_t * yrectf, const char *format, ...)
{
    yrectf_common_init(YLOGGER_INFO, info_);
    yrectf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
yrectf_dbg ( yrectf_t * yrectf, const char *format, ...)
{
    yrectf_common_init(YLOGGER_DEBUG, dbg_);
    yrectf_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define yrectf yrectf_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    yrectf_common_init(YLOGGER_ERROR, err_);
    yrectf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    yrectf_common_init(YLOGGER_WARNING, wrn_);
    yrectf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    yrectf_common_init(YLOGGER_INFO, info_);
    yrectf_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    yrectf_common_init(YLOGGER_DEBUG, dbg_);
    yrectf_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef yrectf

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
