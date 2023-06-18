#if !defined(CTL_ERROR_H)
#define CTL_ERROR_H

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
// Assert that provided pointer is not NULL.
// Process dies if NULLs are set fatal in compile
// flags.
#define NOT_NULL(ptr)                                                    \
    {                                                                    \
        if (ptr == NULL) {                                               \
            PRINTF_ERROR("%s\n",                                         \
                         "Null pointer error on variable \"" #ptr "\""); \
            NULL_PANIC;                                                  \
        }                                                                \
    }
#else
#define NOT_NULL(ptr) UNDEFINED
#endif

#endif  // CTL_ERROR_H
