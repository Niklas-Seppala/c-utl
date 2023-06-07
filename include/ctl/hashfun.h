#if !defined(CTL_HASHFUN_H)
#define CTL_HASHFUN_H

#include "inttypes.h"
#include "stdlib.h"

typedef uint64_t (*CTLHashFunction)(const void *key, const size_t len);

uint64_t CTLHashDJB2_STRING(const void *key, const size_t len);

uint64_t CTLHashFNV(const void *key, const size_t len);

uint64_t CTLHashSDBM_STRING(const void *key, const size_t len);

#endif  // CTL_HASHFUN_H
