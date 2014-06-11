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
#include <stdint.h>

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
    EXPECT_OK(exitcode);
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
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_EQ(*utf8.value_, 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join (&utf8, 1, "abc");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join (&utf8, 1, "abc", "cde");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join (&utf8, 2, "abc", "def");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abcdef"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join (&utf8, 3, "abc", "def", "ghi");
    EXPECT_OK(exitcode);
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
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_EQ(*utf8.value_, 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join_sep ("/", &utf8, 1, "abc");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join_sep ("/", &utf8, 1, "abc", "cde");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join_sep ("/", &utf8, 2, "abc", "def");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc/def"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join_sep ("/", &utf8, 3, "abc", "def", "ghi");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc/def/ghi"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_join_sep ("/", &utf8, 5, "", "abc", "def", "ghi", "");
    EXPECT_OK(exitcode);
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
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_EQ(*utf8.value_, 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_sprintf (&utf8, "abc");
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_sprintf (&utf8, "abc %d", 1);
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc 1"), 0);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_sprintf (&utf8, "abc %.2f %d", 0.5, 1);
    EXPECT_OK(exitcode);
    EXPECT_FALSE(utf8.value_ == NULL);
    EXPECT_NE(*utf8.value_, 0);
    EXPECT_EQ(strcmp(utf8.value_, "abc 0.50 1"), 0);
    printf("%s\n", utf8.value_);
    ymem_free ((void*)utf8.value_);
    utf8.value_ = NULL;

    exitcode = yutf8_sprintf (&utf8, "abc %s", "def");
    EXPECT_OK(exitcode);
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
    EXPECT_OK(exitcode);
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

/* ------------------------------------------------------------------------- */
yt_func_exit_code_t yutf8_foreach_kb_test (
        yutf8_t * yutf8, uint32_t code_point,
        size_t offset, size_t index, void * user)
{
    EXPECT_EQ(offset, index);
    EXPECT_NULL(user);
    EXPECT_NNULL(yutf8);
    switch (index) {
    case 0: {
        EXPECT_EQ(code_point, 'a');
        break; }
    case 1: {
        EXPECT_EQ(code_point, 'b');
        break; }
    case 2: {
        EXPECT_EQ(code_point, 'c');
        break; }
    case 3: {
        EXPECT_EQ(code_point, 'd');
        break; }
    }
    return YT_FUNC_OK;
}

TEST(yutf8,yutf8_foreach){

    yt_func_exit_code_t exitcode;

    yutf8_t utf8;
    exitcode = yutf8_init (&utf8, "abcd");
    EXPECT_OK(exitcode);
    exitcode = yutf8_foreach (&utf8, yutf8_foreach_kb_test, NULL);
    EXPECT_OK(exitcode);

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

