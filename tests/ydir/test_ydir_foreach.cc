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
#include <yt/ylogger.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifdef TARGET_SYSTEM_WIN32
#define REAL_TREE_DIR "C:"
#else
#define REAL_TREE_DIR getenv("HOME")
#endif

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
typedef struct {
    size_t	files;
    size_t	dirs;
} kb_iter_1_t;

static yt_func_exit_code_t kb_iter_1 (
        ydir_t * ydir, const char * absolute_path,
        const char *name_only, int is_file, void * user)
{
    EXPECT_TRUE(ydir != NULL);
    EXPECT_TRUE(absolute_path != NULL);
    EXPECT_TRUE(name_only != NULL);
    EXPECT_TRUE(user != NULL);
    EXPECT_TRUE(name_only-absolute_path > 0);

    dbg_message("[%d] %s", is_file, absolute_path);

    kb_iter_1_t *data = (kb_iter_1_t *)user;
    if (is_file) {
        data->files++;
    } else {
        data->dirs++;
    }
    return YT_FUNC_OK;
}

/* ------------------------------------------------------------------------- */
TEST(ydir_foreach,generic){

    kb_iter_1_t data = {0,0};
    ydir_t ydir;
    yt_func_exit_code_t exitcode;
    exitcode = ydir_init_dir (&ydir, REAL_TREE_DIR);
    EXPECT_OK(exitcode);

    exitcode = ydir_foreach (
        &ydir,
        YDIR_PATTERN_GROUP,
        YDIR_ITER_FILES_AND_DIRECTORIES, // | YDIR_ITER_RECURSIVE,
        kb_iter_1,
        &data);
    EXPECT_OK(exitcode);
    dbg_message("Files %d", data.files);
    dbg_message("Dirs %d", data.dirs);

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

