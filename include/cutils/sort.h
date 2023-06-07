#if !defined(CUTL_SORT_H)
#define CUTL_SORT_H
#include "cutils/compare.h"
#include "stdlib.h"

// ----------------------------------------------
//              Sort functions
// ----------------------------------------------
void CTLMergeSort(void *array, size_t arraySize, size_t entrySizeInBytes, CTLCompareFunction cmpFunction);

#endif  // CUTL_SORT_H
