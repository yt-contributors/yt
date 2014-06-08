/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			yuuid.c
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

#include "yuuid.h"

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
    if (yuuid_implicit_instance == NULL) { \
        yuuid_new (&yuuid_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static yuuid_t * yuuid_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void yuuid_default_err (yuuid_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yuuid_default_wrn (yuuid_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yuuid_default_info (yuuid_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void yuuid_default_dbg (yuuid_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yuuid_init_implicit (
        const char * app_name)
{
    return yuuid_new (&yuuid_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yuuid_init (yuuid_t * yuuid, const char * app_name)
{
    yt_func_start;

    memset (yuuid, 0, sizeof(yuuid_t));

    // by default enable all types
    yuuid->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    yuuid->err_ = yuuid_default_err;
    yuuid->wrn_ = yuuid_default_wrn;
    yuuid->info_ = yuuid_default_info;
    yuuid->dbg_ = yuuid_default_dbg;

    // copy app name for our use
    yt_func_null(yuuid->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    yuuid->syslog_ = 1;
    openlog (yuuid->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    yuuid->callbacks_ = 0;
#else
    yuuid->syslog_ = 0;

    // enable logging through callbacks
    yuuid->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yuuid_end (yuuid_t * yuuid)
{
    memset (yuuid, 0, sizeof(yuuid_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
yuuid_new (yuuid_t ** yuuid, const char * app_name)
{
    DBG_ASSERT(yuuid != NULL);\
    yuuid_t * yuuid_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(yuuid_loc, (yuuid_t*)malloc (sizeof(yuuid_t)));

    // and prepare it
    exitcode = yuuid_init (yuuid_loc, app_name);

    yt_func_end;

    *yuuid = yuuid_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yuuid_free (yuuid_t ** yuuid)
{
    DBG_ASSERT(yuuid != NULL);\
    yuuid_t * yuuid_loc = *yuuid;
    if (yuuid_loc != NULL) {
        yuuid_end (yuuid_loc);
        free (yuuid_loc);
        *yuuid = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yuuid_t *
yuuid_implicit ()
{
    enshure_implicit;
    return yuuid_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define yuuid_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(yuuid != NULL);\
    if (!yuuid_is_enabled (yuuid, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((yuuid->callbacks_) && (yuuid->__kb__ != NULL)) {\
        yuuid->__kb__(yuuid, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define yuuid_common_syslog \
    if (yuuid->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define yuuid_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yuuid_err ( yuuid_t * yuuid, const char *format, ...)
{
    yuuid_common_init(YLOGGER_ERROR, err_);
    yuuid_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yuuid_wrn ( yuuid_t * yuuid, const char *format, ...)
{
    yuuid_common_init(YLOGGER_WARNING, wrn_);
    yuuid_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
yuuid_info ( yuuid_t * yuuid, const char *format, ...)
{
    yuuid_common_init(YLOGGER_INFO, info_);
    yuuid_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
yuuid_dbg ( yuuid_t * yuuid, const char *format, ...)
{
    yuuid_common_init(YLOGGER_DEBUG, dbg_);
    yuuid_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define yuuid yuuid_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    yuuid_common_init(YLOGGER_ERROR, err_);
    yuuid_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    yuuid_common_init(YLOGGER_WARNING, wrn_);
    yuuid_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    yuuid_common_init(YLOGGER_INFO, info_);
    yuuid_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    yuuid_common_init(YLOGGER_DEBUG, dbg_);
    yuuid_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef yuuid

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
