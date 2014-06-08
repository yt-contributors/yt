/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ylinkedlist.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ylinkedlist_h_INCLUDE
#define YT_ylinkedlist_h_INCLUDE
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

struct _ylinkedlist_t;

//! a logger function
///
typedef void (*ylinkedlist_func_t) (
        struct _ylinkedlist_t *,
        const char *format,
        va_list);


//! types of messages
///
typedef enum _ylinkedlist_type_t {
    YLOGGER_NONE = 0x0000,

    YLOGGER_ERROR = 0x0001,
    YLOGGER_WARNING = 0x0002,
    YLOGGER_INFO = 0x0004,
    YLOGGER_DEBUG = 0x0008,

    YLOGGER_ALL = YLOGGER_ERROR | YLOGGER_WARNING | YLOGGER_INFO | YLOGGER_DEBUG,

} ylinkedlist_type_t;


//! a logger structure
///
typedef struct _ylinkedlist_t {

    const char * app_name_;     /**< the reported name of the application */

    int syslog_;                /**< log to syslog if available (0 disabled, 1 enabled) */
    int callbacks_;             /**< log to callbacks if available (0 disabled, 1 enabled) */

    int enabled_mask_;          /**< tells which types are enabled */

    // the callbacks
    ylinkedlist_func_t err_;        /**< pointer to function that does error reporting */
    ylinkedlist_func_t wrn_;        /**< pointer to function that does warning reporting */
    ylinkedlist_func_t info_;       /**< pointer to function that does information reporting */
    ylinkedlist_func_t dbg_;        /**< pointer to function that does debug reporting */

} ylinkedlist_t;


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
#define YLOGGER_WRAP(__name__) ylinkedlist_ ## __name__
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
ylinkedlist_init_implicit (
        const char * app_name);

//! get implicit instance
///
/// Creates a new instance if none exists
///
YLOGGER_EXPORT struct _ylinkedlist_t *
ylinkedlist_implicit ();

//! initialize
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ylinkedlist_init (
        struct _ylinkedlist_t * ylinkedlist,
        const char * app_name);

//! terminate
///
YLOGGER_EXPORT void
ylinkedlist_end (
        struct _ylinkedlist_t * ylinkedlist);

//! initialize
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ylinkedlist_new (
        struct _ylinkedlist_t ** ylinkedlist,
        const char * app_name);

//! terminate
///
YLOGGER_EXPORT void
ylinkedlist_free (
        struct _ylinkedlist_t ** ylinkedlist);


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
ylinkedlist_enable (
        struct _ylinkedlist_t * ylinkedlist,
        ylinkedlist_type_t ty)
{
    ylinkedlist->enabled_mask_ = ylinkedlist->enabled_mask_ | ty;
}

//! disable a kind of message
///
static inline void
ylinkedlist_disable (
        struct _ylinkedlist_t * ylinkedlist,
        ylinkedlist_type_t ty)
{
    ylinkedlist->enabled_mask_ = ylinkedlist->enabled_mask_ & (~ty);
}


//! tell if a kind is enabled or not
///
static inline int
ylinkedlist_is_enabled (
        struct _ylinkedlist_t * ylinkedlist,
        ylinkedlist_type_t ty)
{
    return ((ylinkedlist->enabled_mask_ & ty) == ty);
}

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Explicit
 *  Methods that explicitly provide ylinkedlist_t
 */
///@{


//! report an error
///
YLOGGER_EXPORT void
ylinkedlist_err (
        struct _ylinkedlist_t * ylinkedlist,
        const char *format,
        ...);

//! report a warning
///
YLOGGER_EXPORT void
ylinkedlist_wrn (
        struct _ylinkedlist_t * ylinkedlist,
        const char *format,
        ...);

//! report an information
///
YLOGGER_EXPORT void
ylinkedlist_info (
        struct _ylinkedlist_t * ylinkedlist,
        const char *format,
        ...);

//! report debug information
///
#ifdef YT_DEBUG
    YLOGGER_EXPORT void
    ylinkedlist_dbg (
            struct _ylinkedlist_t * ylinkedlist,
            const char *format,
            ...);
#else // YT_DEBUG
    static inline void
    ylinkedlist_dbg (
            struct _ylinkedlist_t * ylinkedlist,
            const char *format,
            ...) {
        do { (void)(ylinkedlist); (void)(format); } while (0);
    }
#endif // YT_DEBUG

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Implicit
 *  Methods that use default ylinkedlist_t
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
#endif // YT_ylinkedlist_h_INCLUDE
