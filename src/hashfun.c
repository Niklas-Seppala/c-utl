#include "cutils/hashfun.h"

#include "cutils.h"
#include "stdlib.h"

uint64_t CTLHashDJB2_STRING(const void *key, const size_t len) {
    UNUSED_PARAMETER(len);
    const uint8_t *str = key;
    uint64_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

uint64_t CTLHashFNV(const void *key, const size_t len) {
    const uint8_t *p = key;
    uint64_t h = 0xcbf29ce484222325;
    for (size_t i = 0; i < len; i++)
        h = (h ^ p[i]) * 0x100000001b3;

    return h;
}

uint64_t CTLHashSDBM_STRING(const void *key, const size_t len) {
    UNUSED_PARAMETER(len);
    const uint8_t *str = key;
    uint64_t hash = 0;
    int c;
    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}