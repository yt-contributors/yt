/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include "../testhelpers.h"

#include <yt/ybitarray.h>

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
TEST(ybitarray,generic){
    yt_func_exit_code_t exitcode;
    ybitarray_t ybit;

    exitcode = ybitarray_init (&ybit);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    ybitarray_end (&ybit);

    exitcode = ybitarray_init_bits (&ybit, "12121212", 4);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    exitcode = ybitarray_set (&ybit, 0, 1);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_NE(ybitarray_get(&ybit, 0), 0);
    exitcode = ybitarray_set (&ybit, 1, 0);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_EQ(ybitarray_get(&ybit, 1), 0);
    exitcode = ybitarray_set (&ybit, 2, 1);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_NE(ybitarray_get(&ybit, 2), 0);
    exitcode = ybitarray_set (&ybit, 3, 1);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_NE(ybitarray_get(&ybit, 3), 0);
    ybitarray_end (&ybit);

    exitcode = ybitarray_init_bytes (&ybit, "12121212", 4);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    ybitarray_end (&ybit);



}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

