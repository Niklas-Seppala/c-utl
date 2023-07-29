#include "ctl/sort.h"

inline int CTLCompareInt(const void* a, const void* b) {
    if (a == NULL && b == NULL) return EQ;
    if (a != NULL && b == NULL) return GT;
    if (a == NULL && b != NULL) return LT;

    int result = *((int*)a) - *((int*)b);
    if (result == EQ) return EQ;
    if (result < EQ) return LT;
    if (result > EQ) return GT;
    return 0;
}

inline int CTLCompareFloat(const void* a, const void* b) {
    if (a == NULL && b == NULL) return EQ;
    if (a != NULL && b == NULL) return GT;
    if (a == NULL && b != NULL) return LT;
    return *((float*)a) - *((float*)b);
}

inline int CTLCompareLong(const void* a, const void* b) {
    if (a == NULL && b == NULL) return EQ;
    if (a != NULL && b == NULL) return GT;
    if (a == NULL && b != NULL) return LT;

    return *((long*)a) - *((long*)b);
}

inline int CTLCompareChar(const void* a, const void* b) {
    if (a == NULL && b == NULL) return EQ;
    if (a != NULL && b == NULL) return GT;
    if (a == NULL && b != NULL) return LT;

    return *((char*)a) - *((char*)b);
}

inline int CTLCompareDouble(const void* a, const void* b) {
    if (a == NULL && b == NULL) return EQ;
    if (a != NULL && b == NULL) return GT;
    if (a == NULL && b != NULL) return LT;

    return *((double*)a) - *((double*)b);
}

inline bool CTLPredicateNotNull(const void* item) { return item != NULL; }

int CTLCompareString(const void* a, const void* b) {
    if (a == NULL && b == NULL) return EQ;
    if (a != NULL && b == NULL) return GT;
    if (a == NULL && b != NULL) return LT;

    const unsigned char* aStr = a;
    const unsigned char* bStr = b;
    unsigned char aSum, bSum;
    do {
        aSum = *aStr++;
        bSum = *bStr++;
        if (aSum == 0) {
            return aSum - bSum;
        }
    } while (aSum == bSum);
    return aSum - bSum;
}

inline int CTLCompareStringPointer(const void* a, const void* b) {
    return CTLCompareString(*(unsigned char**)a, *(unsigned char**)b);
}