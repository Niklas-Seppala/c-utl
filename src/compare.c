#include "cutils/sort.h"

inline int CTLCompareInt(const void* a, const void* b)
{
    return *((int*)a) - *((int*)b);
}

inline int CTLCompareFloat(const void* a, const void* b)
{
    return *((float*)a) - *((float*)b);
}

inline int CTLCompareLong(const void* a, const void* b)
{
    return *((long*)a) - *((long*)b);
}

inline int CTLCompareChar(const void* a, const void* b)
{
    return *((char*)a) - *((char*)b);
}

inline int CTLCompareDouble(const void* a, const void* b)
{
    return *((double*)a) - *((double*)b);
}

int CTLCompareStringPointer(const void* a, const void* b)
{
    unsigned char* aStr = *(unsigned char**)a;
    unsigned char* bStr = *(unsigned char**)b;
    unsigned char aSum, bSum;
    do
    {
        aSum = *aStr++;
        bSum = *bStr++;
        if (aSum == 0)
        {
            return aSum - bSum;
        }
    } while (aSum == bSum);
    return aSum - bSum;
}