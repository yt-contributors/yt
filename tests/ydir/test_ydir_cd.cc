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
#ifndef TARGET_SYSTEM_WIN32
TEST(ydir_cd,generic){

    ydir_t ydir;
    yt_func_exit_code_t exitcode;
    exitcode = ydir_init_dir (&ydir, ABSOLUTE_PATH_START "a" YDIR_PATH_SEP "b" YDIR_PATH_SEP "c" YDIR_PATH_SEP "d");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), ABSOLUTE_PATH_START "a" YDIR_PATH_SEP "b" YDIR_PATH_SEP "c" YDIR_PATH_SEP "d");

    exitcode = ydir_cd (&ydir, "e");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), ABSOLUTE_PATH_START "a" YDIR_PATH_SEP "b" YDIR_PATH_SEP "c" YDIR_PATH_SEP "d" YDIR_PATH_SEP "e");

    exitcode = ydir_cd (&ydir, ABSOLUTE_PATH_START "x" YDIR_PATH_SEP "y" YDIR_PATH_SEP "z");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), ABSOLUTE_PATH_START "x" YDIR_PATH_SEP "y" YDIR_PATH_SEP "z");

    exitcode = ydir_cd (&ydir, ".");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), ABSOLUTE_PATH_START "x" YDIR_PATH_SEP "y" YDIR_PATH_SEP "z");

    exitcode = ydir_cd (&ydir, "..");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), ABSOLUTE_PATH_START "x" YDIR_PATH_SEP "y" );

    exitcode = ydir_cd (&ydir, "..");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), ABSOLUTE_PATH_START "x" );

    exitcode = ydir_cd (&ydir, "..");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), YDIR_PATH_SEP);

    exitcode = ydir_cd (&ydir, "..");
    EXPECT_EQ(exitcode, YT_FUNC_BAD_INPUT);
    EXPECT_STREQ(ydir_path (&ydir), YDIR_PATH_SEP );

    exitcode = ydir_cd (&ydir, "y" YDIR_PATH_SEP "z");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), ABSOLUTE_PATH_START "y" YDIR_PATH_SEP "z");

    exitcode = ydir_cd (&ydir, ".." YDIR_PATH_SEP ".." YDIR_PATH_SEP "t" YDIR_PATH_SEP "k");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), ABSOLUTE_PATH_START "t" YDIR_PATH_SEP "k");

    exitcode = ydir_cd (&ydir, "." YDIR_PATH_SEP ".." YDIR_PATH_SEP "g");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), ABSOLUTE_PATH_START "t" YDIR_PATH_SEP "g");

    ydir_end (&ydir);

}
#endif
/* ========================================================================= */



/* ------------------------------------------------------------------------- */
#ifdef TARGET_SYSTEM_WIN32
TEST(ydir_cd,generic){

    ydir_t ydir;
    yt_func_exit_code_t exitcode;
    exitcode = ydir_init_dir (&ydir, "a:" YDIR_PATH_SEP "b" YDIR_PATH_SEP "c" YDIR_PATH_SEP "d");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), "a:" YDIR_PATH_SEP "b" YDIR_PATH_SEP "c" YDIR_PATH_SEP "d");

    exitcode = ydir_cd (&ydir, "e");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), "a:" YDIR_PATH_SEP "b" YDIR_PATH_SEP "c" YDIR_PATH_SEP "d" YDIR_PATH_SEP "e");

    exitcode = ydir_cd (&ydir, "x:" YDIR_PATH_SEP "y" YDIR_PATH_SEP "z");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), "x:" YDIR_PATH_SEP "y" YDIR_PATH_SEP "z");

    exitcode = ydir_cd (&ydir, ".");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), "x:" YDIR_PATH_SEP "y" YDIR_PATH_SEP "z");

    exitcode = ydir_cd (&ydir, "..");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), "x:" YDIR_PATH_SEP "y" );

    exitcode = ydir_cd (&ydir, "..");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), "x:" );

    exitcode = ydir_cd (&ydir, "..");
    EXPECT_EQ(exitcode, YT_FUNC_BAD_INPUT);
    EXPECT_STREQ(ydir_path (&ydir), "x:" );

    exitcode = ydir_cd (&ydir, "y" YDIR_PATH_SEP "z");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), "x:" YDIR_PATH_SEP "y" YDIR_PATH_SEP "z");

    exitcode = ydir_cd (&ydir, ".." YDIR_PATH_SEP ".." YDIR_PATH_SEP "t" YDIR_PATH_SEP "k");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), "x:" YDIR_PATH_SEP "t" YDIR_PATH_SEP "k");

    exitcode = ydir_cd (&ydir, "." YDIR_PATH_SEP ".." YDIR_PATH_SEP "g");
    EXPECT_OK(exitcode);
    EXPECT_STREQ(ydir_path (&ydir), "x:" YDIR_PATH_SEP "t" YDIR_PATH_SEP "g");

    ydir_end (&ydir);

}
#endif
/* ========================================================================= */






/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

