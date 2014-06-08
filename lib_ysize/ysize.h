/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ysize.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ysize_h_INCLUDE
#define YT_ysize_h_INCLUDE
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include <yt/yt.h>

#ifdef __cplusplus
extern "C" {
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

struct _ysize_t;

//! a logger function
///
typedef void (*ysize_func_t) (
        struct _ysize_t *,
        const char *format,
        va_list);


//! types of messages
///
typedef enum _ysize_type_t {
    YLOGGER_NONE = 0x0000,

    YLOGGER_ERROR = 0x0001,
    YLOGGER_WARNING = 0x0002,
    YLOGGER_INFO = 0x0004,
    YLOGGER_DEBUG = 0x0008,

    YLOGGER_ALL = YLOGGER_ERROR | YLOGGER_WARNING | YLOGGER_INFO | YLOGGER_DEBUG,

} ysize_type_t;


//! a logger structure
///
typedef struct _ysize_t {

    const char * app_name_;     /**< the reported name of the application */

    int syslog_;                /**< log to syslog if available (0 disabled, 1 enabled) */
    int callbacks_;             /**< log to callbacks if available (0 disabled, 1 enabled) */

    int enabled_mask_;          /**< tells which types are enabled */

    // the callbacks
    ysize_func_t err_;        /**< pointer to function that does error reporting */
    ysize_func_t wrn_;        /**< pointer to function that does warning reporting */
    ysize_func_t info_;       /**< pointer to function that does information reporting */
    ysize_func_t dbg_;        /**< pointer to function that does debug reporting */

} ysize_t;


//! may be used to add special directives to export function
/// @{
#ifndef YLOGGER_EXPORT
#define YLOGGER_EXPORT
#endif
/// @}


//! not yet implemented
/// @{
#ifndef YLOGGER_IMPLEMENT_ME
#define YLOGGER_IMPLEMENT_ME
#endif
/// @}


//! simple functions are outside the namespace by default
/// @{
#ifdef YLOGGER_WRAP_ALL
#define YLOGGER_WRAP(__name__) ysize_ ## __name__
#else
#define YLOGGER_WRAP(__name__) __name__
#endif
/// @}


/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Base
 *  Constructors, destructors and so on
 */
///@{

//! initialize implicit instance
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ysize_init_implicit (
        const char * app_name);

//! get implicit instance
///
/// Creates a new instance if none exists
///
YLOGGER_EXPORT struct _ysize_t *
ysize_implicit ();

//! initialize
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ysize_init (
        struct _ysize_t * ysize,
        const char * app_name);

//! terminate
///
YLOGGER_EXPORT void
ysize_end (
        struct _ysize_t * ysize);

//! initialize
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ysize_new (
        struct _ysize_t ** ysize,
        const char * app_name);

//! terminate
///
YLOGGER_EXPORT void
ysize_free (
        struct _ysize_t ** ysize);


///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Enable/Disable
 *  enabling and diasabling types of messages
 */
///@{


//! enable a kind of message
///
static inline void
ysize_enable (
        struct _ysize_t * ysize,
        ysize_type_t ty)
{
    ysize->enabled_mask_ = ysize->enabled_mask_ | ty;
}

//! disable a kind of message
///
static inline void
ysize_disable (
        struct _ysize_t * ysize,
        ysize_type_t ty)
{
    ysize->enabled_mask_ = ysize->enabled_mask_ & (~ty);
}


//! tell if a kind is enabled or not
///
static inline int
ysize_is_enabled (
        struct _ysize_t * ysize,
        ysize_type_t ty)
{
    return ((ysize->enabled_mask_ & ty) == ty);
}

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Explicit
 *  Methods that explicitly provide ysize_t
 */
///@{


//! report an error
///
YLOGGER_EXPORT void
ysize_err (
        struct _ysize_t * ysize,
        const char *format,
        ...);

//! report a warning
///
YLOGGER_EXPORT void
ysize_wrn (
        struct _ysize_t * ysize,
        const char *format,
        ...);

//! report an information
///
YLOGGER_EXPORT void
ysize_info (
        struct _ysize_t * ysize,
        const char *format,
        ...);

//! report debug information
///
#ifdef YT_DEBUG
    YLOGGER_EXPORT void
    ysize_dbg (
            struct _ysize_t * ysize,
            const char *format,
            ...);
#else // YT_DEBUG
    static inline void
    ysize_dbg (
            struct _ysize_t * ysize,
            const char *format,
            ...) {
        do { (void)(ysize); (void)(format); } while (0);
    }
#endif // YT_DEBUG

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Implicit
 *  Methods that use default ysize_t
 */
///@{


//! report an error
///
YLOGGER_EXPORT void
YLOGGER_WRAP(err_message) (
        const char *format,
        ...);

//! report a warning
///
YLOGGER_EXPORT void
YLOGGER_WRAP(wrn_message) (
        const char *format,
        ...);

//! report an information
///
YLOGGER_EXPORT void
YLOGGER_WRAP(info_message) (
        const char *format,
        ...);

//! report debug information
///
#ifdef YT_DEBUG
    YLOGGER_EXPORT void
    YLOGGER_WRAP(dbg_message) (
            const char *format,
            ...);    
#else // YT_DEBUG
static inline void
    YLOGGER_WRAP(dbg_message) (
            const char *format,
            ...) {
        do { (void)(format); } while (0);
    }
#endif // YT_DEBUG

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifdef __cplusplus
}
#endif
#endif // YT_ysize_h_INCLUDE
