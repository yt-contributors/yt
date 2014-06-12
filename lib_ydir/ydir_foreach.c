/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ydir_foreach.c
  \date			June 2014
  \author		Nicu Tofan

  \brief		Implements ydir_foreach function in ydir library


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "ydir.h"
#include "ydir_internal.h"

#include <string.h>

// architecture specific
#ifdef TARGET_SYSTEM_WIN32
#   define WIN32_LEAN_AND_MEAN
#   ifdef _MSC_VER
#       pragma warning (disable : 4996)
#   endif
#   include <windows.h>
#else
#   define _XOPEN_SOURCE 500
#   define __USE_XOPEN_EXTENDED
#   include <sys/stat.h>
#   include <dirent.h>
#   include <regex.h>
#endif

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define LOCAL_BUFFER_SIZE 512

typedef struct _ydir_foreach_data_t {
    // initial arguments
    ydir_t * ydir;
    const char * pattern;
    ydir_iter_flags_t flags;
    ydir_iter_kb kb;
    void * user;

    // derivate variables
    size_t original_len;
    const char * file_name;
    yt_func_exit_code_t exitcode;

    // the buffer for storing absolute path of current item
    char * p_buffer;
    size_t allocated_sz;
    size_t used_sz;
    char loc_buff[LOCAL_BUFFER_SIZE];

    // architecture specific
#   ifdef TARGET_SYSTEM_WIN32

#   else
    regex_t rex;

#   endif


} ydir_foreach_data_t;

#define IS_BUFFER_DYNAMIC(__p__) ((__p__)->allocated_sz > 0)

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
yt_func_exit_code_t
ydir_ensure_size (ydir_foreach_data_t * data, size_t req_size)
{
    char * ptr_tmp;
    req_size += 4;
    if (IS_BUFFER_DYNAMIC(data)) {
        if (data->allocated_sz > req_size) return YT_FUNC_OK;
        ptr_tmp = ymem_realloc (data->p_buffer, req_size);
        if (ptr_tmp == NULL) return YT_FUNC_MEMORY_ERROR;
    } else {
        if (LOCAL_BUFFER_SIZE > req_size) return YT_FUNC_OK;
        ptr_tmp = ymem_malloc (req_size);
        if (ptr_tmp == NULL) return YT_FUNC_MEMORY_ERROR;
        memcpy (ptr_tmp, data->p_buffer, data->used_sz);
    }
    data->p_buffer = ptr_tmp;
    data->allocated_sz = req_size;
    return YT_FUNC_OK;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
yt_func_exit_code_t
ydir_append_utf8 (ydir_foreach_data_t * data, const char * new_str)
{
    size_t new_len = strlen (new_str);
    size_t prev_u = data->used_sz;
    yt_func_exit_code_t exitcode = ydir_ensure_size (data, data->used_sz + new_len + 1);
    if (yt_failure(exitcode)) return exitcode;
    char * dest = data->p_buffer + prev_u;
    //dest[0] = YDIR_PATH_SEP_C; ++dest;
    memcpy (dest, new_str, new_len);
    dest[new_len] = 0;
    return YT_FUNC_OK;
}
/* ========================================================================= */

#ifdef TARGET_SYSTEM_WIN32
/* ------------------------------------------------------------------------- */
yt_func_exit_code_t
ydir_foreach_win (ydir_foreach_data_t * data)
{
    HANDLE h_find;
    WIN32_FIND_DATA find_data;

    // we want to return the buffer in same state as we've got it
    size_t used_data = data->used_sz;

    // append a path separator
    { // don't use end_of_dir below, the buffer may be rellocated
        char *end_of_dir = data->p_buffer + data->used_sz;
        *end_of_dir = YDIR_PATH_SEP_C; end_of_dir++; data->used_sz++;
        *end_of_dir = 0;
    }

    // first, dig the childs
    if ( (data->flags & YDIR_ITER_RECURSIVE) ) {

        // copy the pattern at the end by trick it so that it gets overwritten
        if ( (data->flags & YDIR_ITER_ALL_DIRECTORIES) == 0 ) {
            size_t used_data_pattern = data->used_sz;
            data->exitcode = ydir_append_utf8 (data, data->pattern);
            if ( data->exitcode != YT_FUNC_OK ) {
                return data->exitcode;
            }
            data->used_sz = used_data_pattern;
        } else {
            data->p_buffer[data->used_sz] = '*';
            data->p_buffer[data->used_sz+1] = 0;
        }

        // start the search
        h_find = FindFirstFileEx(data->p_buffer, FindExInfoStandard, &find_data,
             FindExSearchLimitToDirectories, NULL, 0);
        if ( h_find == INVALID_HANDLE_VALUE ) {
            return 0;
        }

        // for each directory
        do { if ( is_entry_directory (&find_data) ) {
            // append the name of the directory
            data->exitcode = char_buff_add(&data->chb, find_data.cFileName);
            if ( data->exitcode != YT_FUNC_OK ) {
                break;
            }

            // inform the callback
            if ( (data->flags & YDIR_ITER_EXCLUDE_DIRECTORIES) == 0 ) {
                data->exitcode = data->kb (
                    data,
                    data->p_buffer,
                    data->p_buffer+used_data+1,
                    data->user_data,
                    0);
                if ( data->exitcode != YT_FUNC_OK ) {
                    break;
                }
            }

            // and call this function again
            data->exitcode = ydir_foreach_win (data);
            if ( data->exitcode != YT_FUNC_OK ) {
                break;
            }
        } } while ( FindNextFile (h_find, &find_data) );

        // close the search and exit if error
        FindClose (h_find);
        if ( data->exitcode != YT_FUNC_OK ) {
            return data->exitcode;
        }
    } // if ( (data->flags & YDIR_ITER_RECURSIVE) )

    // now loop in kids
    if ( (data->flags & YDIR_ITER_EXCLUDE_FILES) == 0 ) {

        // copy the pattern at the end by trick it so that it gets overwritten
        if ( (data->flags & YDIR_ITER_ALL_FILES) == 0 ) {
            size_t used_data_pattern = data->used_sz;
            data->exitcode = char_buff_add(&data->chb, data->pattern);
            if ( data->exitcode != YT_FUNC_OK ) {
                return data->exitcode;
            }
            data->used_sz = used_data_pattern;
        } else {
            data->p_buffer[data->used_sz] = '*';
            data->p_buffer[data->used_sz+1] = 0;
        }

        // start the search
        h_find = FindFirstFileEx(data->p_buffer, FindExInfoStandard, &find_data,
             FindExSearchNameMatch, NULL, 0);
        if ( h_find == INVALID_HANDLE_VALUE ) {
            return 0;
        }

        // for each file
        do { if ( is_entry_file (&find_data) ) {
            // append the name of the file
            data->exitcode = char_buff_add(&data->chb, find_data.cFileName);
            if ( data->exitcode != YT_FUNC_OK ) {
                break;
            }

            // inform the callback
            data->exitcode = data->kb (
                data,
                data->p_buffer,
                data->p_buffer+used_data+1,
                data->user_data,
                1);
            if ( data->exitcode != YT_FUNC_OK ) {
                break;
            }

        } } while ( FindNextFile (h_find, &find_data) );

        // close the search and exit if error
        FindClose (h_find);
        if ( data->exitcode != YT_FUNC_OK ) {
            return data->exitcode;
        }
    } // if ( (data->flags & YDIR_ITER_EXCLUDE_FILES) == 0 )

    // restore the buffer to its former glorry
    data->used_sz = used_data;
    data->p_buffer[used_data] = 0;

    return data->exitcode;
}
/* ========================================================================= */
#endif

#ifndef TARGET_SYSTEM_WIN32
/* ------------------------------------------------------------------------- */
static inline int is_dot_dot (struct dirent *dent) {
    for (;;) {
        if ( dent->d_name[0] == '.' ) {
            if ( dent->d_name[1] == 0 ) {
                break;
            } else if ( dent->d_name[1] == '.' ) {
                if ( dent->d_name[2] == 0 )
                    break;
            }
        }
        return 0;
    }
    return 1;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
yt_func_exit_code_t
ydir_foreach_posix (ydir_foreach_data_t * data)
{
    DIR *dir = NULL;
    struct dirent *dent;
    struct stat st;

    // we want to return the buffer in same state as we've got it
    size_t used_data = data->used_sz;

    // append a path separator
    { // don't use end_of_dir below, the buffer may be rellocated
        char *end_of_dir = data->p_buffer + data->used_sz;
        *end_of_dir = YDIR_PATH_SEP_C; end_of_dir++; data->used_sz++;
        *end_of_dir = 0;
    }

    // open the directory
    dir = opendir (data->p_buffer);
    if (dir == NULL) {
        return YT_FUNC_GENERIC_ERROR;
    }

    // loop in all entries except . and .., symlinks
    while ( (dent = readdir (dir)) ) {
        data->used_sz = used_data + 1;
        if (is_dot_dot (dent))
            continue;

        // copy this name after the path;
        data->exitcode = ydir_append_utf8 (data, dent->d_name);
        if (data->exitcode != YT_FUNC_OK) {
            break;
        }
        data->file_name = data->p_buffer + used_data + 1;

        // get file info; if symbolic link, don't follow
        if (lstat (data->p_buffer, &st) == -1)
            continue; // silently ignoring the error
        if (S_ISLNK(st.st_mode))
            continue;
        if (S_ISDIR(st.st_mode)) {
            // pattern match if they don't get all included
            if ( (data->flags & YDIR_ITER_ALL_DIRECTORIES) == 0 ) {
                if (regexec (&data->rex, dent->d_name, 0, 0, 0))
                    continue;
            }

            // inform the callback
            if ( (data->flags & YDIR_ITER_EXCLUDE_DIRECTORIES) == 0 ) {
                data->exitcode = data->kb (
                            data->ydir,
                            data->p_buffer,
                            data->file_name,
                            0,
                            data->user);
                if ( data->exitcode != YT_FUNC_OK ) {
                    break;
                }
            }

            // recursive? dive in...
            if ( (data->flags & YDIR_ITER_RECURSIVE) ) {
                data->exitcode = ydir_foreach_posix (data);
                if ( data->exitcode != YT_FUNC_OK ) {
                    break;
                }
            }
        } else if ( (data->flags & YDIR_ITER_EXCLUDE_FILES) == 0 ) {

            // pattern match
            if ( (data->flags & YDIR_ITER_ALL_FILES) == 0 ) {
                if (regexec (&data->rex, dent->d_name, 0, 0, 0)) {
                    continue;
                }
            }

            // inform the callback
            data->exitcode = data->kb (
                        data->ydir,
                        data->p_buffer,
                        data->file_name,
                        1,
                        data->user);
            if ( data->exitcode != YT_FUNC_OK ) {
                break;
            }
        }
    } // while ( (dent = readdir (dir)) )


    // done with this directory
    if (dir)
        closedir(dir);

    // restore the buffer to its former glorry
    data->used_sz = used_data;
    data->p_buffer[used_data] = 0;

    return data->exitcode;
}
/* ========================================================================= */
#endif // TARGET_SYSTEM_WIN32

/* ------------------------------------------------------------------------- */
YDIR_EXPORT yt_func_exit_code_t
ydir_foreach(
        ydir_t * ydir, const char * pattern,
        int flags, ydir_iter_kb kb, void * user)
{
    yt_func_exit_code_t exitcode;

    // get arguments inside
    ydir_foreach_data_t data;
    memset (&data, 0, sizeof(ydir_foreach_data_t));
    data.ydir = ydir; if (ydir == NULL) return YT_FUNC_BAD_INPUT;
    if (ystring_is_empty (&ydir->path_)) return YT_FUNC_OK;
    data.pattern = pattern; if (pattern == NULL) data.pattern = YDIR_PATTERN_GROUP;
    data.flags = (ydir_iter_flags_t)flags;
    data.kb = kb; if (kb == NULL) return YT_FUNC_BAD_INPUT;
    data.user = user;
    data.p_buffer = data.loc_buff;

    // copy original path inside our buffer
    data.original_len = ystring_len(&ydir->path_);
    exitcode = ydir_ensure_size (&data, data.original_len + 128);
    if (yt_failure(exitcode)) return exitcode;
    memcpy (data.p_buffer, ydir->path_.buffer_, ydir->path_.bytes_used_);

    // make sure the path does not end in a separator
    if (data.p_buffer[data.original_len-1] == YDIR_PATH_SEP_C) {
        --data.original_len;
        data.p_buffer[data.original_len] = 0;
    }
    data.used_sz = data.original_len;

    for (;;) { // architecture specific

#     ifdef TARGET_SYSTEM_WIN32
        exitcode = ydir_foreach_win (&data);
#     else
        // compile regular expression
        if (regcomp (&data.rex, data.pattern, REG_ICASE | REG_NOSUB)) {
            dbg_message ("regcomp failed");
            break;
        }
        exitcode = ydir_foreach_posix (&data);
        regfree (&data.rex);
#     endif

        break;
    }

    if (IS_BUFFER_DYNAMIC(&data)) {
        ymem_free (data.p_buffer);
    }
    return YT_FUNC_OK;
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
