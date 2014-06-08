/* ========================================================================= */
/* ------------------------------------------------------------------------- */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "gtest/gtest.h"
#include "../testhelpers.h"

#include <yt/yutf8.h>
#include <yt/ymem.h>

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
TEST(yutf8,generic){

    yt_func_exit_code_t exitcode;
    yutf8_t utf8;
    const char * test_str = "abc";
    exitcode = yutf8_init (&utf8, test_str);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_TRUE(utf8.value_ == test_str);

    yutf8_end (&utf8);
    EXPECT_TRUE(utf8.value_ == NULL);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(yutf8,yutf8_join){

    yt_func_exit_code_t exitcode;

    yutf8_t utf8;
    exitcode = yutf8_init (&utf8, NULL);


    exitcode = yutf8_join (NULL, 01);
    EXPECT_EQ(exitcode, YT_FUNC_BAD_INPUT);

    exitcode = yutf8_join (&utf8, 0);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_EQ(*utf8.value_, 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join (&utf8, 1, "abc");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join (&utf8, 1, "abc", "cde");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join (&utf8, 2, "abc", "def");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abcdef"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join (&utf8, 3, "abc", "def", "ghi");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abcdefghi"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    yutf8_end (&utf8);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(yutf8,yutf8_join_sep){

    yt_func_exit_code_t exitcode;

    yutf8_t utf8;
    exitcode = yutf8_init (&utf8, NULL);

    exitcode = yutf8_join_sep ("/", NULL, 01);
    EXPECT_EQ(exitcode, YT_FUNC_BAD_INPUT);

    exitcode = yutf8_join_sep ("/", &utf8, 0);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_EQ(*utf8.value_, 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join_sep ("/", &utf8, 1, "abc");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join_sep ("/", &utf8, 1, "abc", "cde");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join_sep ("/", &utf8, 2, "abc", "def");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc/def"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join_sep ("/", &utf8, 3, "abc", "def", "ghi");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc/def/ghi"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join_sep ("/", &utf8, 5, "", "abc", "def", "ghi", "");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "/abc/def/ghi/"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    yutf8_end (&utf8);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
TEST(yutf8,yutf8_sprintf){

    yt_func_exit_code_t exitcode;

    yutf8_t utf8;
    exitcode = yutf8_init (&utf8, NULL);

    exitcode = yutf8_sprintf (NULL, "/");
    EXPECT_EQ(exitcode, YT_FUNC_BAD_INPUT);

    exitcode = yutf8_sprintf (&utf8, NULL);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_EQ(*utf8.value_, 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_sprintf (&utf8, "abc");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_sprintf (&utf8, "abc %d", 1);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc 1"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_sprintf (&utf8, "abc %.2f %d", 0.5, 1);
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc 0.50 1"), 0);
    printf("%s\n", utf8.value_);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_sprintf (&utf8, "abc %s", "def");
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc def"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_sprintf (
                &utf8,
                "abc %s%s%s%s%s%s%s%s",
                "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef",
                "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef",
                "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef",
                "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef",
                "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef",
                "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef",
                "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef",
                "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef"
                );
    EXPECT_EQ(exitcode, YT_FUNC_OK);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(
                  utf8.value_,
                  "abc "
                  "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef"
                  "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef"
                  "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef"
                  "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef"
                  "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef"
                  "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef"
                  "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef"
                  "defdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdefdef"
                  ), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;



    yutf8_end (&utf8);
}
/* ========================================================================= */


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */

