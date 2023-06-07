#include "ctl/sort.h"

#include "assert.h"
#include "stdio.h"
#include "string.h"

static inline void copyFromArray(char* dest, const char* src, size_t* mergeIndex,
                                 size_t* srcIndex, size_t entrySizeInBytes) {
    memcpy(dest + *mergeIndex, src + *srcIndex, entrySizeInBytes);
    *srcIndex += entrySizeInBytes;
}

static inline void __CTLCopyRemaining(char* dest, const char* src, size_t* mergeIndex,
                                      size_t* srcIndex, size_t srcInBytes,
                                      size_t entrySizeInBytes) {
    while (*srcIndex < srcInBytes) {
        copyFromArray(dest, src, mergeIndex, srcIndex, entrySizeInBytes);
        *mergeIndex += entrySizeInBytes;
    }
}

static void __CTLMerge(char* array, size_t entrySizeInBytes, size_t left,
                       size_t split, size_t right, CTLCompareFunction cmp) {
    const size_t leftArrayByteSize = (split - left + 1) * entrySizeInBytes;
#ifdef CTL_MERGESORT_DYNAMIC_ALLOC
    char* leftArray = malloc(leftArrayByteSize);
#else
    char leftStatic[leftArrayByteSize];
    char* leftArray = leftStatic;
#endif

    const size_t rightArrayByteSize = (right - split) * entrySizeInBytes;
#ifdef CTL_MERGESORT_DYNAMIC_ALLOC
    char* rightArray = malloc(leftArrayByteSize);
#else
    char rStatic[leftArrayByteSize];
    char* rightArray = rStatic;
#endif

    for (size_t i = 0; i < leftArrayByteSize; i += entrySizeInBytes) {
        void* src = array + left * entrySizeInBytes + i;
        void* dest = leftArray + i;
        memcpy(dest, src, entrySizeInBytes);
    }

    for (size_t i = 0; i < rightArrayByteSize; i += entrySizeInBytes) {
        void* src = array + (split + 1) * entrySizeInBytes + i;
        void* dest = rightArray + i;
        memcpy(dest, src, entrySizeInBytes);
    }

    size_t leftIndex = 0, rightIndex = 0;
    size_t mergeIndex = left * entrySizeInBytes;
    while (leftIndex < leftArrayByteSize && rightIndex < rightArrayByteSize) {
        if (cmp(leftArray + leftIndex, rightArray + rightIndex) <= 0)
            copyFromArray(array, leftArray, &mergeIndex, &leftIndex, entrySizeInBytes);
        else
            copyFromArray(array, rightArray, &mergeIndex, &rightIndex, entrySizeInBytes);

        mergeIndex += entrySizeInBytes;
    }

    __CTLCopyRemaining(array, leftArray, &mergeIndex, &leftIndex, leftArrayByteSize, entrySizeInBytes);
    __CTLCopyRemaining(array, rightArray, &mergeIndex, &rightIndex, rightArrayByteSize, entrySizeInBytes);

#ifdef CTL_MERGESORT_DYNAMIC_ALLOC
    free(leftArray);
    free(rightArray);
#endif
}

static void __CTLMergeSort(void* array, size_t entrySizeInBytes, size_t left, size_t right, CTLCompareFunction cmp) {
    if (left < right) {
        size_t split = left + (right - left) / 2;
        __CTLMergeSort(array, entrySizeInBytes, left, split, cmp);
        __CTLMergeSort(array, entrySizeInBytes, split + 1, right, cmp);
        __CTLMerge(array, entrySizeInBytes, left, split, right, cmp);
    }
}

void CTLMergeSort(void* array, size_t arraySize, size_t entrySizeInBytes, CTLCompareFunction cmp) {
    __CTLMergeSort(array, entrySizeInBytes, 0, arraySize - 1, cmp);
}