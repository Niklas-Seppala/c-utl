#if !defined(CTL_COMPARE_H)
#define CTL_COMPARE_H
#include <stdbool.h>

typedef int (*CTLCompareFunction)(const void* a, const void* b);
typedef bool (*CTLpredicate)(const void *);

#define EQUAL 0;


#endif // CTL_COMPARE_H
