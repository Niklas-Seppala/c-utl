#if !defined(CUTL_SORT_H)
#define CUTL_SORT_H
#include "stdlib.h"
#include "cutils/compare.h"

// ----------------------------------------------
//         Comparison functions
// ----------------------------------------------
int CTLCompareInt(const void* a, const void* b);

int CTLCompareFloat(const void* a, const void *b);

int CTLCompareLong(const void* a, const void *b);

int CTLCompareChar(const void* a, const void *b);

int CTLCompareLong(const void* a, const void *b);

int CTLCompareStringPointer(const void *a, const void *b);


// ----------------------------------------------
//              Sort functions
// ----------------------------------------------
void CTLMergeSort(void *array, size_t arraySize, size_t entrySizeInBytes, CTLCompareFunction cmpFunction);

#endif // CUTL_SORT_H
