/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yset.c
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

#include "yset.h"

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
    if (yset_implicit_instance == NULL) { \
        yset_new (&yset_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static yset_t * yset_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void yset_default_err (yset_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yset_default_wrn (yset_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yset_default_info (yset_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yset_default_dbg (yset_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yset_init_implicit (
        const char * app_name)
{
    return yset_new (&yset_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yset_init (yset_t * yset, const char * app_name)
{
    yt_func_start;

    memset (yset, 0, sizeof(yset_t));

    // by default enable all types
    yset->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    yset->err_ = yset_default_err;
    yset->wrn_ = yset_default_wrn;
    yset->info_ = yset_default_info;
    yset->dbg_ = yset_default_dbg;

    // copy app name for our use
    yt_func_null(yset->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    yset->syslog_ = 1;
    openlog (yset->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    yset->callbacks_ = 0;
#else
    yset->syslog_ = 0;

    // enable logging through callbacks
    yset->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yset_end (yset_t * yset)
{
    memset (yset, 0, sizeof(yset_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yset_new (yset_t ** yset, const char * app_name)
{
    DBG_ASSERT(yset != NULL);\
    yset_t * yset_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(yset_loc, (yset_t*)malloc (sizeof(yset_t)));

    // and prepare it
    exitcode = yset_init (yset_loc, app_name);

    yt_func_end;

    *yset = yset_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yset_free (yset_t ** yset)
{
    DBG_ASSERT(yset != NULL);\
    yset_t * yset_loc = *yset;
    if (yset_loc != NULL) {
        yset_end (yset_loc);
        free (yset_loc);
        *yset = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yset_t *
yset_implicit ()
{
    enshure_implicit;
    return yset_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define yset_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(yset != NULL);\
    if (!yset_is_enabled (yset, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((yset->callbacks_) && (yset->__kb__ != NULL)) {\
        yset->__kb__(yset, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define yset_common_syslog \
    if (yset->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define yset_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yset_err ( yset_t * yset, const char *format, ...)
{
    yset_common_init(YLOGGER_ERROR, err_);
    yset_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yset_wrn ( yset_t * yset, const char *format, ...)
{
    yset_common_init(YLOGGER_WARNING, wrn_);
    yset_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yset_info ( yset_t * yset, const char *format, ...)
{
    yset_common_init(YLOGGER_INFO, info_);
    yset_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
yset_dbg ( yset_t * yset, const char *format, ...)
{
    yset_common_init(YLOGGER_DEBUG, dbg_);
    yset_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define yset yset_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    yset_common_init(YLOGGER_ERROR, err_);
    yset_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    yset_common_init(YLOGGER_WARNING, wrn_);
    yset_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    yset_common_init(YLOGGER_INFO, info_);
    yset_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    yset_common_init(YLOGGER_DEBUG, dbg_);
    yset_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef yset

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
