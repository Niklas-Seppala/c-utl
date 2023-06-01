#if !defined(CTL_HASHMAP_H)
#define CTL_HASHMAP_H
#include "stdlib.h"
#include "stdbool.h"
#include "hashfun.h"

typedef struct __CTLHashMap *CTLHashMap;

#ifdef DEBUG
void CTLHashMapDebugPrint(CTLHashMap map);
#endif

CTLHashMap CTLHashMapCreate(const int initialSize, const float loadFactor, CTLHashFunction hashFunction);
void CTLHashMapFree(CTLHashMap map);

int CTLHashMapSize(CTLHashMap map);

bool CTLHashMapIsEmpty(CTLHashMap map);
bool CTLHashMapContains(CTLHashMap map, void *key, size_t keylen);
bool CTLHashMapReplace(CTLHashMap map, const void *key, const size_t keylen, const void *entry);

const void *CTLHashMapGet(CTLHashMap map, const void *key, size_t keylen);
const void *CTLHashMapPut(CTLHashMap map, const void* key, size_t keylen, const void *entry);
const void *CTLHashMapRemove(CTLHashMap map, const void *key, size_t keylen);
const void *CTLHashMapPutIfAbsent(CTLHashMap map, const void *key, const size_t keylen, const void *entry);
void CTLHashMapKeys(CTLHashMap map, void** keys);
void CTLHashMapClear(CTLHashMap map);


#endif // CTL_HASHMAP_H
