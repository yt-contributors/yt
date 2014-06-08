/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include "../testhelpers.h"

#include <yt/ylogger.h>

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
TEST(ylogger_base,generic){

    yt_func_exit_code_t exitcode = ylogger_init_implicit("testapp");
    EXPECT_EQ(exitcode, YT_FUNC_OK);

    err_message ("test error %s", "with string");
    wrn_message ("test warning %s", "with string");
    info_message ("test information %s", "with string");
    dbg_message ("test debug %s", "with string");


    ylogger_t * ylogger_loc;
    exitcode = ylogger_new (&ylogger_loc, "testapp2");
    EXPECT_EQ(exitcode, YT_FUNC_OK);

    ylogger_disable (ylogger_loc, YLOGGER_ERROR);
    EXPECT_EQ(ylogger_is_enabled(ylogger_loc, YLOGGER_ERROR), 0);
    ylogger_disable (ylogger_loc, YLOGGER_WARNING);
    EXPECT_EQ(ylogger_is_enabled(ylogger_loc, YLOGGER_WARNING), 0);
    ylogger_disable (ylogger_loc, YLOGGER_INFO);
    EXPECT_EQ(ylogger_is_enabled(ylogger_loc, YLOGGER_INFO), 0);
    ylogger_disable (ylogger_loc, YLOGGER_DEBUG);
    EXPECT_EQ(ylogger_is_enabled(ylogger_loc, YLOGGER_DEBUG), 0);

    ylogger_enable (ylogger_loc, YLOGGER_ERROR);
    EXPECT_NE(ylogger_is_enabled(ylogger_loc, YLOGGER_ERROR), 0);
    ylogger_enable (ylogger_loc, YLOGGER_WARNING);
    EXPECT_NE(ylogger_is_enabled(ylogger_loc, YLOGGER_WARNING), 0);
    ylogger_enable (ylogger_loc, YLOGGER_INFO);
    EXPECT_NE(ylogger_is_enabled(ylogger_loc, YLOGGER_INFO), 0);
    ylogger_enable (ylogger_loc, YLOGGER_DEBUG);
    EXPECT_NE(ylogger_is_enabled(ylogger_loc, YLOGGER_DEBUG), 0);

    ylogger_err (ylogger_loc, "test error %s", "with string");
    ylogger_wrn (ylogger_loc, "test warning %s", "with string");
    ylogger_info (ylogger_loc, "test information %s", "with string");
    ylogger_dbg (ylogger_loc, "test debug %s", "with string");

    ylogger_free (&ylogger_loc);
    EXPECT_TRUE(ylogger_loc == NULL);

}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

