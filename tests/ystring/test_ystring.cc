/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include "../testhelpers.h"

#include <yt/ystring.h>

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
TEST(ystring,generic){

    yt_func_exit_code_t exitcode;
    ystring_t str1;

    exitcode = ystring_init (&str1, NULL);
    EXPECT_OK(exitcode);
    EXPECT_TRUE(ystring_is_null (&str1));
    EXPECT_TRUE(ystring_is_empty (&str1));
    EXPECT_EQ(ystring_len (&str1), 0);
    ystring_end (&str1);

    exitcode = ystring_init (&str1, "test string");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(ystring_is_null (&str1));
    EXPECT_FALSE(ystring_is_empty (&str1));
    EXPECT_EQ(ystring_len (&str1), strlen ("test string"));
    ystring_end (&str1);

}

/* ------------------------------------------------------------------------- */
TEST(ystring,ystring_init_counted){

    yt_func_exit_code_t exitcode;
    ystring_t str1;

    exitcode = ystring_init_counted (&str1, "test string", 4);
    EXPECT_OK(exitcode);
    EXPECT_FALSE(ystring_is_null (&str1));
    EXPECT_FALSE(ystring_is_empty (&str1));
    EXPECT_EQ(ystring_len (&str1), strlen ("test"));
    ystring_end (&str1);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(ystring,ystring_clear){

    yt_func_exit_code_t exitcode;
    ystring_t str1;

    exitcode = ystring_init (&str1, "test string");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(ystring_is_null (&str1));
    EXPECT_FALSE(ystring_is_empty (&str1));

    ystring_clear (&str1);
    EXPECT_TRUE(ystring_is_empty (&str1));
    EXPECT_EQ(ystring_len (&str1), 0);
    ystring_end (&str1);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(ystring,ystring_set){

    yt_func_exit_code_t exitcode;
    ystring_t str1;

    exitcode = ystring_init (&str1, "test string");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(ystring_is_null(&str1));
    EXPECT_FALSE(ystring_is_empty(&str1));

    exitcode = ystring_set (&str1, "another string");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(ystring_is_null(&str1));
    EXPECT_FALSE(ystring_is_empty(&str1));
    EXPECT_EQ(ystring_len(&str1), strlen ("another string"));
    ystring_end (&str1);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(ystring,ystring_init_c){

    yt_func_exit_code_t exitcode;
    ystring_t str1;
    ystring_t str2;

    exitcode = ystring_init (&str1, "a string");
    EXPECT_OK(exitcode);
    EXPECT_EQ(ystring_len (&str1), strlen ("a string"));
    ystring_init_c (&str2, &str1);
    EXPECT_FALSE(ystring_is_null (&str2));
    EXPECT_FALSE(ystring_is_empty (&str2));
    EXPECT_EQ(ystring_len (&str2), strlen ("a string"));
    ystring_end (&str1);
    ystring_end (&str2);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(ystring,ystring_copy){

    yt_func_exit_code_t exitcode;
    ystring_t str1;
    ystring_t str2;

    exitcode = ystring_init (&str1, "a string");
    EXPECT_EQ(ystring_len (&str1), strlen ("a string"));
    exitcode = ystring_init (&str2, "test string");
    EXPECT_EQ(ystring_len (&str2), strlen ("test string"));
    exitcode = ystring_copy (&str1, &str2);
    EXPECT_OK(exitcode);
    EXPECT_EQ(ystring_len (&str2), strlen ("a string"));
    ystring_end (&str1);
    ystring_end (&str2);

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(ystring, ystring_append){

    yt_func_exit_code_t exitcode;
    ystring_t str1;
    ystring_t str2;
    ystring_t str3;

    ystring_init (&str1, "a string ");
    ystring_init (&str2, "storry that ");
    ystring_init (&str3, "never ends");

    exitcode = ystring_append (&str1, 2, &str2, &str3);
    EXPECT_OK(exitcode);
    EXPECT_EQ(ystring_len (&str1), strlen ("a string storry that never ends"));
    EXPECT_EQ(ystring_len (&str2), strlen ("storry that "));
    EXPECT_EQ(ystring_len (&str3), strlen ("never ends"));

    ystring_end (&str1);
    ystring_end (&str2);
    ystring_end (&str3);

}
/* ========================================================================= */




/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

