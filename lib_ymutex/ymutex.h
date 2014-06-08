/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ymutex.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ymutex_h_INCLUDE
#define YT_ymutex_h_INCLUDE
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

struct _ymutex_t;

//! a logger function
///
typedef void (*ymutex_func_t) (
        struct _ymutex_t *,
        const char *format,
        va_list);


//! types of messages
///
typedef enum _ymutex_type_t {
    YLOGGER_NONE = 0x0000,

    YLOGGER_ERROR = 0x0001,
    YLOGGER_WARNING = 0x0002,
    YLOGGER_INFO = 0x0004,
    YLOGGER_DEBUG = 0x0008,

    YLOGGER_ALL = YLOGGER_ERROR | YLOGGER_WARNING | YLOGGER_INFO | YLOGGER_DEBUG,

} ymutex_type_t;


//! a logger structure
///
typedef struct _ymutex_t {

    const char * app_name_;     /**< the reported name of the application */

    int syslog_;                /**< log to syslog if available (0 disabled, 1 enabled) */
    int callbacks_;             /**< log to callbacks if available (0 disabled, 1 enabled) */

    int enabled_mask_;          /**< tells which types are enabled */

    // the callbacks
    ymutex_func_t err_;        /**< pointer to function that does error reporting */
    ymutex_func_t wrn_;        /**< pointer to function that does warning reporting */
    ymutex_func_t info_;       /**< pointer to function that does information reporting */
    ymutex_func_t dbg_;        /**< pointer to function that does debug reporting */

} ymutex_t;


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
#define YLOGGER_WRAP(__name__) ymutex_ ## __name__
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
ymutex_init_implicit (
        const char * app_name);

//! get implicit instance
///
/// Creates a new instance if none exists
///
YLOGGER_EXPORT struct _ymutex_t *
ymutex_implicit ();

//! initialize
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ymutex_init (
        struct _ymutex_t * ymutex,
        const char * app_name);

//! terminate
///
YLOGGER_EXPORT void
ymutex_end (
        struct _ymutex_t * ymutex);

//! initialize
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ymutex_new (
        struct _ymutex_t ** ymutex,
        const char * app_name);

//! terminate
///
YLOGGER_EXPORT void
ymutex_free (
        struct _ymutex_t ** ymutex);


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
ymutex_enable (
        struct _ymutex_t * ymutex,
        ymutex_type_t ty)
{
    ymutex->enabled_mask_ = ymutex->enabled_mask_ | ty;
}

//! disable a kind of message
///
static inline void
ymutex_disable (
        struct _ymutex_t * ymutex,
        ymutex_type_t ty)
{
    ymutex->enabled_mask_ = ymutex->enabled_mask_ & (~ty);
}


//! tell if a kind is enabled or not
///
static inline int
ymutex_is_enabled (
        struct _ymutex_t * ymutex,
        ymutex_type_t ty)
{
    return ((ymutex->enabled_mask_ & ty) == ty);
}

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Explicit
 *  Methods that explicitly provide ymutex_t
 */
///@{


//! report an error
///
YLOGGER_EXPORT void
ymutex_err (
        struct _ymutex_t * ymutex,
        const char *format,
        ...);

//! report a warning
///
YLOGGER_EXPORT void
ymutex_wrn (
        struct _ymutex_t * ymutex,
        const char *format,
        ...);

//! report an information
///
YLOGGER_EXPORT void
ymutex_info (
        struct _ymutex_t * ymutex,
        const char *format,
        ...);

//! report debug information
///
#ifdef YT_DEBUG
    YLOGGER_EXPORT void
    ymutex_dbg (
            struct _ymutex_t * ymutex,
            const char *format,
            ...);
#else // YT_DEBUG
    static inline void
    ymutex_dbg (
            struct _ymutex_t * ymutex,
            const char *format,
            ...) {
        do { (void)(ymutex); (void)(format); } while (0);
    }
#endif // YT_DEBUG

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Implicit
 *  Methods that use default ymutex_t
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
#endif // YT_ymutex_h_INCLUDE
