/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ymap.h
  \date			June 2014
  \author		Nicu Tofan


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef YT_ymap_h_INCLUDE
#define YT_ymap_h_INCLUDE
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

struct _ymap_t;

//! a logger function
///
typedef void (*ymap_func_t) (
        struct _ymap_t *,
        const char *format,
        va_list);


//! types of messages
///
typedef enum _ymap_type_t {
    YLOGGER_NONE = 0x0000,

    YLOGGER_ERROR = 0x0001,
    YLOGGER_WARNING = 0x0002,
    YLOGGER_INFO = 0x0004,
    YLOGGER_DEBUG = 0x0008,

    YLOGGER_ALL = YLOGGER_ERROR | YLOGGER_WARNING | YLOGGER_INFO | YLOGGER_DEBUG,

} ymap_type_t;


//! a logger structure
///
typedef struct _ymap_t {

    const char * app_name_;     /**< the reported name of the application */

    int syslog_;                /**< log to syslog if available (0 disabled, 1 enabled) */
    int callbacks_;             /**< log to callbacks if available (0 disabled, 1 enabled) */

    int enabled_mask_;          /**< tells which types are enabled */

    // the callbacks
    ymap_func_t err_;        /**< pointer to function that does error reporting */
    ymap_func_t wrn_;        /**< pointer to function that does warning reporting */
    ymap_func_t info_;       /**< pointer to function that does information reporting */
    ymap_func_t dbg_;        /**< pointer to function that does debug reporting */

} ymap_t;


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
#define YLOGGER_WRAP(__name__) ymap_ ## __name__
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
ymap_init_implicit (
        const char * app_name);

//! get implicit instance
///
/// Creates a new instance if none exists
///
YLOGGER_EXPORT struct _ymap_t *
ymap_implicit ();

//! initialize
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ymap_init (
        struct _ymap_t * ymap,
        const char * app_name);

//! terminate
///
YLOGGER_EXPORT void
ymap_end (
        struct _ymap_t * ymap);

//! initialize
///
/// Provides the structure with default callbacks.
///
YLOGGER_EXPORT yt_func_exit_code_t
ymap_new (
        struct _ymap_t ** ymap,
        const char * app_name);

//! terminate
///
YLOGGER_EXPORT void
ymap_free (
        struct _ymap_t ** ymap);


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
ymap_enable (
        struct _ymap_t * ymap,
        ymap_type_t ty)
{
    ymap->enabled_mask_ = ymap->enabled_mask_ | ty;
}

//! disable a kind of message
///
static inline void
ymap_disable (
        struct _ymap_t * ymap,
        ymap_type_t ty)
{
    ymap->enabled_mask_ = ymap->enabled_mask_ & (~ty);
}


//! tell if a kind is enabled or not
///
static inline int
ymap_is_enabled (
        struct _ymap_t * ymap,
        ymap_type_t ty)
{
    return ((ymap->enabled_mask_ & ty) == ty);
}

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Explicit
 *  Methods that explicitly provide ymap_t
 */
///@{


//! report an error
///
YLOGGER_EXPORT void
ymap_err (
        struct _ymap_t * ymap,
        const char *format,
        ...);

//! report a warning
///
YLOGGER_EXPORT void
ymap_wrn (
        struct _ymap_t * ymap,
        const char *format,
        ...);

//! report an information
///
YLOGGER_EXPORT void
ymap_info (
        struct _ymap_t * ymap,
        const char *format,
        ...);

//! report debug information
///
#ifdef YT_DEBUG
    YLOGGER_EXPORT void
    ymap_dbg (
            struct _ymap_t * ymap,
            const char *format,
            ...);
#else // YT_DEBUG
    static inline void
    ymap_dbg (
            struct _ymap_t * ymap,
            const char *format,
            ...) {
        do { (void)(ymap); (void)(format); } while (0);
    }
#endif // YT_DEBUG

///@}
// == == == == == == == == == == == == == == == == == == == == == ==


// == == == == == == == == == == == == == == == == == == == == == ==
/** @name Implicit
 *  Methods that use default ymap_t
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
#endif // YT_ymap_h_INCLUDE
