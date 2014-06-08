/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ysizef.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ysizef_h_INCLUDE
#define YT_ysizef_h_INCLUDE
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

struct _ysizef_t;

//! a logger function
///
typedef void (*ysizef_func_t) (
        struct _ysizef_t *,
        const char *format,
        va_list);


//! types of messages
///
typedef enum _ysizef_type_t {
    YLOGGER_NONE = 0x0000,

    YLOGGER_ERROR = 0x0001,
    YLOGGER_WARNING = 0x0002,
    YLOGGER_INFO = 0x0004,
    YLOGGER_DEBUG = 0x0008,

    YLOGGER_ALL = YLOGGER_ERROR | YLOGGER_WARNING | YLOGGER_INFO | YLOGGER_DEBUG,

} ysizef_type_t;


//! a logger structure
///
typedef struct _ysizef_t {

    const char * app_name_;     /**< the reported name of the application */

    int syslog_;                /**< log to syslog if available (0 disabled, 1 enabled) */
    int callbacks_;             /**< log to callbacks if available (0 disabled, 1 enabled) */

    int enabled_mask_;          /**< tells which types are enabled */

    // the callbacks
    ysizef_func_t err_;        /**< pointer to function that does error reporting */
    ysizef_func_t wrn_;        /**< pointer to function that does warning reporting */
    ysizef_func_t info_;       /**< pointer to function that does information reporting */
    ysizef_func_t dbg_;        /**< pointer to function that does debug reporting */

} ysizef_t;


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
#define YLOGGER_WRAP(__name__) ysizef_ ## __name__
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
ysizef_init_implicit (
        const char * app_name);

//! get implicit instance
///
/// Creates a new instance if none exists
///
YLOGGER_EXPORT struct _ysizef_t *
ysizef_implicit ();

//! initialize
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ysizef_init (
        struct _ysizef_t * ysizef,
        const char * app_name);

//! terminate
///
YLOGGER_EXPORT void
ysizef_end (
        struct _ysizef_t * ysizef);

//! initialize
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ysizef_new (
        struct _ysizef_t ** ysizef,
        const char * app_name);

//! terminate
///
YLOGGER_EXPORT void
ysizef_free (
        struct _ysizef_t ** ysizef);


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
ysizef_enable (
        struct _ysizef_t * ysizef,
        ysizef_type_t ty)
{
    ysizef->enabled_mask_ = ysizef->enabled_mask_ | ty;
}

//! disable a kind of message
///
static inline void
ysizef_disable (
        struct _ysizef_t * ysizef,
        ysizef_type_t ty)
{
    ysizef->enabled_mask_ = ysizef->enabled_mask_ & (~ty);
}


//! tell if a kind is enabled or not
///
static inline int
ysizef_is_enabled (
        struct _ysizef_t * ysizef,
        ysizef_type_t ty)
{
    return ((ysizef->enabled_mask_ & ty) == ty);
}

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Explicit
 *  Methods that explicitly provide ysizef_t
 */
///@{


//! report an error
///
YLOGGER_EXPORT void
ysizef_err (
        struct _ysizef_t * ysizef,
        const char *format,
        ...);

//! report a warning
///
YLOGGER_EXPORT void
ysizef_wrn (
        struct _ysizef_t * ysizef,
        const char *format,
        ...);

//! report an information
///
YLOGGER_EXPORT void
ysizef_info (
        struct _ysizef_t * ysizef,
        const char *format,
        ...);

//! report debug information
///
#ifdef YT_DEBUG
    YLOGGER_EXPORT void
    ysizef_dbg (
            struct _ysizef_t * ysizef,
            const char *format,
            ...);
#else // YT_DEBUG
    static inline void
    ysizef_dbg (
            struct _ysizef_t * ysizef,
            const char *format,
            ...) {
        do { (void)(ysizef); (void)(format); } while (0);
    }
#endif // YT_DEBUG

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Implicit
 *  Methods that use default ysizef_t
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
#endif // YT_ysizef_h_INCLUDE
