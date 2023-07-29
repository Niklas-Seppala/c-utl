#include <stdarg.h>
#include <stdlib.h>

#include "ctl/error.h"

#ifdef RT_NULL_CHECKS

#define NULL_CHECK(ptr)                                                  \
    {                                                                    \
        if (ptr == NULL) {                                               \
            PRINTF_ERROR("%s\n",                                         \
                         "Null pointer error on variable \"" #ptr "\""); \
            NULL_PANIC;                                                  \
        }                                                                \
    }

void CTL_NOT_NULL(void *first, ...) {
    va_list args;
    va_start(args, first);
    void *ptr = NULL;
    while ((ptr = va_arg(args, void *))) {
        NULL_CHECK(ptr);
    }
    va_end(args);
}
#endif