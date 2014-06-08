/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yvector.c
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

#include "yvector.h"

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
    if (yvector_implicit_instance == NULL) { \
        yvector_new (&yvector_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static yvector_t * yvector_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void yvector_default_err (yvector_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yvector_default_wrn (yvector_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yvector_default_info (yvector_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yvector_default_dbg (yvector_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yvector_init_implicit (
        const char * app_name)
{
    return yvector_new (&yvector_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yvector_init (yvector_t * yvector, const char * app_name)
{
    yt_func_start;

    memset (yvector, 0, sizeof(yvector_t));

    // by default enable all types
    yvector->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    yvector->err_ = yvector_default_err;
    yvector->wrn_ = yvector_default_wrn;
    yvector->info_ = yvector_default_info;
    yvector->dbg_ = yvector_default_dbg;

    // copy app name for our use
    yt_func_null(yvector->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    yvector->syslog_ = 1;
    openlog (yvector->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    yvector->callbacks_ = 0;
#else
    yvector->syslog_ = 0;

    // enable logging through callbacks
    yvector->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yvector_end (yvector_t * yvector)
{
    memset (yvector, 0, sizeof(yvector_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yvector_new (yvector_t ** yvector, const char * app_name)
{
    DBG_ASSERT(yvector != NULL);\
    yvector_t * yvector_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(yvector_loc, (yvector_t*)malloc (sizeof(yvector_t)));

    // and prepare it
    exitcode = yvector_init (yvector_loc, app_name);

    yt_func_end;

    *yvector = yvector_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yvector_free (yvector_t ** yvector)
{
    DBG_ASSERT(yvector != NULL);\
    yvector_t * yvector_loc = *yvector;
    if (yvector_loc != NULL) {
        yvector_end (yvector_loc);
        free (yvector_loc);
        *yvector = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yvector_t *
yvector_implicit ()
{
    enshure_implicit;
    return yvector_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define yvector_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(yvector != NULL);\
    if (!yvector_is_enabled (yvector, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((yvector->callbacks_) && (yvector->__kb__ != NULL)) {\
        yvector->__kb__(yvector, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define yvector_common_syslog \
    if (yvector->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define yvector_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yvector_err ( yvector_t * yvector, const char *format, ...)
{
    yvector_common_init(YLOGGER_ERROR, err_);
    yvector_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yvector_wrn ( yvector_t * yvector, const char *format, ...)
{
    yvector_common_init(YLOGGER_WARNING, wrn_);
    yvector_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yvector_info ( yvector_t * yvector, const char *format, ...)
{
    yvector_common_init(YLOGGER_INFO, info_);
    yvector_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
yvector_dbg ( yvector_t * yvector, const char *format, ...)
{
    yvector_common_init(YLOGGER_DEBUG, dbg_);
    yvector_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define yvector yvector_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    yvector_common_init(YLOGGER_ERROR, err_);
    yvector_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    yvector_common_init(YLOGGER_WARNING, wrn_);
    yvector_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    yvector_common_init(YLOGGER_INFO, info_);
    yvector_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    yvector_common_init(YLOGGER_DEBUG, dbg_);
    yvector_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef yvector

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
