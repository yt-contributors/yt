/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ystringlist.c
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

#include "ystringlist.h"

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
    if (ystringlist_implicit_instance == NULL) { \
        ystringlist_new (&ystringlist_implicit_instance, ""); \
    } \

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static ystringlist_t * ystringlist_implicit_instance = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void ystringlist_default_err (ystringlist_t * logger, const char *format, va_list arg)
{
    default_reporting("ERROR!!!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ystringlist_default_wrn (ystringlist_t * logger, const char *format, va_list arg)
{
    default_reporting("Warning!");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ystringlist_default_info (ystringlist_t * logger, const char *format, va_list arg)
{
    default_reporting("Info:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void ystringlist_default_dbg (ystringlist_t * logger, const char *format, va_list arg)
{
    default_reporting("  Debug:");
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ystringlist_init_implicit (
        const char * app_name)
{
    return ystringlist_new (&ystringlist_implicit_instance, app_name);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ystringlist_init (ystringlist_t * ystringlist, const char * app_name)
{
    yt_func_start;

    memset (ystringlist, 0, sizeof(ystringlist_t));

    // by default enable all types
    ystringlist->enabled_mask_ = YLOGGER_ALL;

    // set default callbacks
    ystringlist->err_ = ystringlist_default_err;
    ystringlist->wrn_ = ystringlist_default_wrn;
    ystringlist->info_ = ystringlist_default_info;
    ystringlist->dbg_ = ystringlist_default_dbg;

    // copy app name for our use
    yt_func_null(ystringlist->app_name_, strdup (app_name));

    // enable logging through syslog
#ifdef YT_SYSLOG_FOUND
    ystringlist->syslog_ = 1;
    openlog (ystringlist->app_name_, LOG_PERROR | LOG_CONS | LOG_NDELAY, LOG_USER); // | LOG_PID

    // disable logging through callbacks
    ystringlist->callbacks_ = 0;
#else
    ystringlist->syslog_ = 0;

    // enable logging through callbacks
    ystringlist->callbacks_ = 1;
#endif // SYSLOG_FOUND

    yt_func_end;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ystringlist_end (ystringlist_t * ystringlist)
{
    memset (ystringlist, 0, sizeof(ystringlist_t));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT yt_func_exit_code_t
ystringlist_new (ystringlist_t ** ystringlist, const char * app_name)
{
    DBG_ASSERT(ystringlist != NULL);\
    ystringlist_t * ystringlist_loc = NULL;

    yt_func_start;

    // allocate a new chunk
    yt_func_null(ystringlist_loc, (ystringlist_t*)malloc (sizeof(ystringlist_t)));

    // and prepare it
    exitcode = ystringlist_init (ystringlist_loc, app_name);

    yt_func_end;

    *ystringlist = ystringlist_loc;

    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ystringlist_free (ystringlist_t ** ystringlist)
{
    DBG_ASSERT(ystringlist != NULL);\
    ystringlist_t * ystringlist_loc = *ystringlist;
    if (ystringlist_loc != NULL) {
        ystringlist_end (ystringlist_loc);
        free (ystringlist_loc);
        *ystringlist = NULL;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT ystringlist_t *
ystringlist_implicit ()
{
    enshure_implicit;
    return ystringlist_implicit_instance;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#define ystringlist_common_init(__msgconst__, __kb__) \
 \
    DBG_ASSERT(ystringlist != NULL);\
    if (!ystringlist_is_enabled (ystringlist, __msgconst__)) return;\
    \
    va_list argptr;\
    va_start (argptr, format);\
\
    if ((ystringlist->callbacks_) && (ystringlist->__kb__ != NULL)) {\
        ystringlist->__kb__(ystringlist, format, argptr);\
    }

#ifdef YT_SYSLOG_FOUND
#   define ystringlist_common_syslog \
    if (ystringlist->syslog_) { \
        vsyslog (LOG_MAKEPRI(LOG_USER, LOG_NOTICE), format, argptr); \
    }
#else
#   define ystringlist_common_syslog
#endif // SYSLOG_FOUND
/* ========================================================================= */


/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ystringlist_err ( ystringlist_t * ystringlist, const char *format, ...)
{
    ystringlist_common_init(YLOGGER_ERROR, err_);
    ystringlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ystringlist_wrn ( ystringlist_t * ystringlist, const char *format, ...)
{
    ystringlist_common_init(YLOGGER_WARNING, wrn_);
    ystringlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
ystringlist_info ( ystringlist_t * ystringlist, const char *format, ...)
{
    ystringlist_common_init(YLOGGER_INFO, info_);
    ystringlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
ystringlist_dbg ( ystringlist_t * ystringlist, const char *format, ...)
{
    ystringlist_common_init(YLOGGER_DEBUG, dbg_);
    ystringlist_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#define ystringlist ystringlist_implicit_instance

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) ( const char *format, ...)
{
    enshure_implicit;
    ystringlist_common_init(YLOGGER_ERROR, err_);
    ystringlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) ( const char *format, ...)
{
    enshure_implicit;
    ystringlist_common_init(YLOGGER_WARNING, wrn_);
    ystringlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) ( const char *format, ...)
{
    enshure_implicit;
    ystringlist_common_init(YLOGGER_INFO, info_);
    ystringlist_common_syslog;
    va_end (argptr);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
YLOGGER_EXPORT void
YLOGGER_WRAP(dbg_message) ( const char *format, ...)
{
    enshure_implicit;
    ystringlist_common_init(YLOGGER_DEBUG, dbg_);
    ystringlist_common_syslog;
    va_end (argptr);
}
#endif // YT_DEBUG
/* ========================================================================= */

#undef ystringlist

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
