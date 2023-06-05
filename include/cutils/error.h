#include "stdio.h"
#include "cutils/terminal.h"
#include "cutils.h"

// Prints error with information where it happened.
#define PRINTF_ERROR(format, ...) \
{ \
    printf(TERM_RED format, __VA_ARGS__); \
    printf("\tat %s(), %s:%d"TERM_RSET"\n", __func__, __FILE__, __LINE__); \
} \


#ifdef RT_NULL_CHECKS
    // Process aborts on unexpected null pointer.
    #define NULL_PANIC abort();
#else 
    #define NULL_PANIC UNDEFINED
#endif

#ifdef RT_NULL_CHECKS
    // Assert that provided pointer is not NULL.
    // Process dies if NULLs are set fatal in compile
    // flags.
    #define NOT_NULL(ptr) \
    { \
        if (ptr == NULL) \
        { \
            PRINTF_ERROR("%s\n", "Null pointer error on variable \"" #ptr "\""); \
            NULL_PANIC; \
        } \
    }
#else
    #define NOT_NULL(ptr) UNDEFINED
#endif