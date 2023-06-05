#if !defined(CTL_COMPARE_H)
#define CTL_COMPARE_H
#include <stdbool.h>

typedef int (*CTLCompareFunction)(const void* a, const void* b);
typedef bool (*CTLpredicate)(const void *);
typedef const char *((*CTLObjectToString)(const void*));

// ----------------------------------------------
//         Comparison functions
// ----------------------------------------------
int CTLCompareInt(const void* a, const void* b);

int CTLCompareFloat(const void* a, const void *b);

int CTLCompareLong(const void* a, const void *b);

int CTLCompareChar(const void* a, const void *b);

int CTLCompareLong(const void* a, const void *b);

int CTLCompareStringPointer(const void *a, const void *b);

int CTLCompareString(const void* a, const void* b);

bool CTLPredicateNotNull(const void *item);

#define EQ (0)
#define LT (-1)
#define GT (1)


#endif // CTL_COMPARE_H
