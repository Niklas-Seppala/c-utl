#include "cutils/math.h"

inline int roundToPow2(int n) {
    int result = 1;
    while ((result <<= 1) < n) ;
    return result;
}


inline int greaterOf(int a, int b) {
    return a >= b ? a : b;
}