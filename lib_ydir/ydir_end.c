/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ydir_end.c
  \date			June 2014
  \author		Nicu Tofan

  \brief		Implements ydir_end function in ydir library


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "ydir.h"

#include <string.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

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

/* ------------------------------------------------------------------------- */
YDIR_EXPORT void
YDIR_IMPLEMENT_ME ydir_end(ydir_t * ydir)
{
    if (ydir->internal_path_ != NULL) free (ydir->internal_path_);
    memset (ydir, 0, sizeof(ydir_t));
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
