/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file			ycntbuff.c
  \date			June 2014
  \author		Nicu Tofan

  \brief


*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "ycntbuff.h"
#include <yt/ymem.h>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#define ycntbuff_user_buffer(__p__)  (((char*)(__p__)) + sizeof(ycntbuff_t))
#define ycntbuff_from_user_buffer(__p__)  (ycntbuff_t*)(((char*)(__p__)) - sizeof(ycntbuff_t))

#ifdef YT_DEBUG
static size_t ycntbuff_find_owner( ycntbuff_t * buf_hd, const void * owner);
static void ycntbuff_add_owner( ycntbuff_t * buf_hd, const void * owner);
#endif // YT_DEBUG

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
YCNTBUFF_EXPORT yt_func_exit_code_t
ycntbuff_new (
        void ** buffer_holder, const void * owner, size_t usable_size)
{
    if (buffer_holder == NULL) return YT_FUNC_BAD_INPUT;
    if (owner == NULL) return YT_FUNC_BAD_INPUT;

    ycntbuff_t * ret = NULL;
    void * user_ret = NULL;
    size_t allocated_size = usable_size + sizeof(ycntbuff_t);

    yt_func_start;

    // allocate the buffer with a proper header
    yt_func_null(ret, (ycntbuff_t*)ymem_malloc(allocated_size));
    ret->count_ = 1;
    user_ret = ycntbuff_user_buffer(ret);

#   ifdef YT_DEBUG
    // save debug information
    yt_func_null(ret->owners_list_, (void**)ymem_malloc (sizeof(void*)*16));
    ret->allocated_sz_ = 16;
    ret->used_sz_ = 1;
    ret->owners_list_[0] = (void*)owner;
#   endif // YT_DEBUG

    // prevent dealocation in the end
    ret = NULL;

    yt_func_end;
    if (ret != NULL) ymem_free (ret);
    *buffer_holder = user_ret;
    yt_func_ret;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YCNTBUFF_EXPORT void
ycntbuff_dec ( void ** buffer_holder, const void * owner)
{
    if (buffer_holder == NULL) {
        YT_BREAKPOINT;
        return;
    }
    if (owner == NULL) {
        YT_BREAKPOINT;
        return;
    }

    // get back out pointer
    ycntbuff_t * ret = ycntbuff_from_user_buffer(*buffer_holder);
    DBG_ASSERT(ret->count_ > 0);

#   ifdef YT_DEBUG
    // locate this owner
    size_t owner_index = ycntbuff_find_owner (ret, owner);
    if (owner_index == ret->used_sz_) {
        err_message ("Provided owner %lld is not part "
                     "of the owners list for %lld",
                     (long)owner, (long)*buffer_holder);
        YT_BREAKPOINT;
    }
#   endif // YT_DEBUG

    // decrement the reference counter
    --ret->count_;
    if (ret->count_ == 0) {
#       ifdef YT_DEBUG
        ymem_free (ret->owners_list_);
#       endif // YT_DEBUG
        ymem_free (ret);
    } else {
#       ifdef YT_DEBUG
        ret->owners_list_[owner_index] = NULL;
#       endif // YT_DEBUG
    }

    // clear the holder
    *buffer_holder = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YCNTBUFF_EXPORT void
ycntbuff_inc (void * buffer, const void * owner)
{
    if (buffer == NULL) {
        YT_BREAKPOINT;
        return;
    }
    if (owner == NULL) {
        YT_BREAKPOINT;
        return;
    }

    // get back out pointer
    ycntbuff_t * ret = ycntbuff_from_user_buffer(buffer);
    DBG_ASSERT(ret->count_ > 0);

    // change the reference count
    ++ret->count_;

#   ifdef YT_DEBUG
    ycntbuff_add_owner (ret, owner);
#   endif // YT_DEBUG

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
YCNTBUFF_EXPORT size_t
ycntbuff_count ( const void * buffer)
{
    if (buffer == NULL) return 0;
    ycntbuff_t * ret = ycntbuff_from_user_buffer(buffer);
    DBG_ASSERT(ret->count_ > 0);
    return ret->count_;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
static size_t
ycntbuff_find_owner( ycntbuff_t * buf_hd, const void * owner)
{
    void ** iter = buf_hd->owners_list_;
    size_t i;
    for (i = 0; i < buf_hd->used_sz_; ++i) {
        if (owner == *iter) return i;
        ++iter;
    }
    return buf_hd->used_sz_;
}
#endif // YT_DEBUG
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
#ifdef YT_DEBUG
static void
ycntbuff_add_owner( ycntbuff_t * buf_hd, const void * owner)
{
    void ** iter = buf_hd->owners_list_;
    size_t i;

    // fill in blank spots
    if (buf_hd->freed_sz_ > 0) {
        for (i = 0; i < buf_hd->used_sz_; ++i) {
            if (NULL == *iter) {
                *iter = (void*)owner;
                return;
            }
            ++iter;
        }
        iter = buf_hd->owners_list_;
        buf_hd->freed_sz_ = 0;
    }

    // add it to the end
    if (buf_hd->used_sz_ < buf_hd->allocated_sz_ ) {
        iter[buf_hd->used_sz_++] = (void*)owner;
        return;
    }

    // enlarge the buffer
    size_t new_sz = buf_hd->allocated_sz_*2;
    iter = (void **)realloc (buf_hd->owners_list_, new_sz);
    if (iter == NULL) {
        YT_BREAKPOINT;
    } else {
        buf_hd->owners_list_ = iter;
        buf_hd->allocated_sz_ = new_sz;
        iter[buf_hd->used_sz_++] = (void*)owner;
    }
}
#endif // YT_DEBUG
/* ========================================================================= */



/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
