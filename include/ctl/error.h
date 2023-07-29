#if !defined(CTL_ERROR_H)
#define CTL_ERROR_H

#include <stdarg.h>

#include "ctl.h"
#include "ctl/terminal.h"
#include "stdio.h"

// Prints error with information where it happened.
#define PRINTF_ERROR(format, ...)                                     \
    {                                                                 \
        printf(TERM_RED format, __VA_ARGS__);                         \
        printf("\tat %s(), %s:%d" TERM_RSET "\n", __func__, __FILE__, \
               __LINE__);                                             \
    }

#ifdef RT_NULL_CHECKS
// Process aborts on unexpected null pointer.
#define NULL_PANIC abort();
#else
#define NULL_PANIC UNDEFINED
#endif

#ifdef RT_INDEX_BOUNDS_CHECKS
#define INDEX_IN_BOUNDS(index, lower, upper)                               \
    {                                                                      \
        if (index < lower || index > upper) {                              \
            PRINTF_ERROR("%s %ld, bounds are %ld - %ld\n",                 \
                         "Index out of bounds error on variable \"" #index \
                         "\"",                                             \
                         index, lower, upper);                             \
            abort();                                                       \
        }                                                                  \
    }
#define INDEX_IN_UPPER_BOUND(index, upper)                                 \
    {                                                                      \
        if (index >= upper) {                                              \
            PRINTF_ERROR("%s %ld, upper bound is %ld\n",                   \
                         "Index out of bounds error on variable \"" #index \
                         "\"",                                             \
                         index, upper);                                    \
            abort();                                                       \
        }                                                                  \
    }
#define INDEX_IN_LOWER_BOUND(index, lower)                                 \
    {                                                                      \
        if (index < lower) {                                               \
            PRINTF_ERROR("%s %ld, lower bound is %ld\n",                   \
                         "Index out of bounds error on variable \"" #index \
                         "\"",                                             \
                         index, lower);                                    \
            abort();                                                       \
        }                                                                  \
    }
#else
#define INDEX_IN_BOUNDS(index, a, b) UNDEFINED
#endif

#ifdef RT_NULL_CHECKS
void CTL_NOT_NULL(void *first, ...);
#define NOT_NULL(...) \
    { CTL_NOT_NULL(NULL, __VA_ARGS__); }
#endif

#endif  // CTL_ERROR_H
