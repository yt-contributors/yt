/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include "../testhelpers.h"

#include <yt/ydir.h>

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
TEST(ydir_init_dir,generic){

    ydir_t ydir;
    yt_func_exit_code_t exitcode;
    exitcode = ydir_init_dir (&ydir, YDIR_PATH_SEP "a" YDIR_PATH_SEP "b" YDIR_PATH_SEP "c" YDIR_PATH_SEP "d");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), YDIR_PATH_SEP "a" YDIR_PATH_SEP "b" YDIR_PATH_SEP "c" YDIR_PATH_SEP "d");
    ydir_end (&ydir);
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

