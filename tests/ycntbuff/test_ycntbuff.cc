/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include "../testhelpers.h"

#include <yt/ycntbuff.h>

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
TEST(ycntbuff,generic){

    void * buffer_holder;
    void * x_buffer_holder;
    const char * owner = "some pointer";
    yt_func_exit_code_t exitcode;

    exitcode = ycntbuff_new (&buffer_holder, owner, 152);
    EXPECT_OK(exitcode);
    EXPECT_NNULL(buffer_holder);
    EXPECT_EQ(ycntbuff_count(buffer_holder), 1);
    ycntbuff_dec (&buffer_holder, owner);
    EXPECT_NULL(buffer_holder);

    exitcode = ycntbuff_new (&buffer_holder, owner, 152*152);
    x_buffer_holder = buffer_holder;
    EXPECT_OK(exitcode);
    EXPECT_NNULL(buffer_holder);
    EXPECT_EQ(ycntbuff_count(buffer_holder), 1);
    ycntbuff_inc (buffer_holder, owner);
    EXPECT_EQ(ycntbuff_count(buffer_holder), 2);
    ycntbuff_inc (buffer_holder, owner);
    EXPECT_EQ(ycntbuff_count(buffer_holder), 3);
    ycntbuff_inc (buffer_holder, owner);
    EXPECT_EQ(ycntbuff_count(buffer_holder), 4);
    ycntbuff_inc (buffer_holder, owner);
    EXPECT_EQ(ycntbuff_count(buffer_holder), 5);

    ycntbuff_dec (&buffer_holder, owner);
    EXPECT_NULL(buffer_holder);
    buffer_holder = x_buffer_holder;
    EXPECT_EQ(ycntbuff_count(buffer_holder), 4);

    ycntbuff_dec (&buffer_holder, owner);
    EXPECT_NULL(buffer_holder);
    buffer_holder = x_buffer_holder;
    EXPECT_EQ(ycntbuff_count(buffer_holder), 3);

    ycntbuff_dec (&buffer_holder, owner);
    EXPECT_NULL(buffer_holder);
    buffer_holder = x_buffer_holder;
    EXPECT_EQ(ycntbuff_count(buffer_holder), 2);

    ycntbuff_dec (&buffer_holder, owner);
    EXPECT_NULL(buffer_holder);
    buffer_holder = x_buffer_holder;
    EXPECT_EQ(ycntbuff_count(buffer_holder), 1);

    ycntbuff_dec (&buffer_holder, owner);
    EXPECT_NULL(buffer_holder);
    buffer_holder = x_buffer_holder;

}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

