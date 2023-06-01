#include "cutils/hashfun.h"
#include "cutils/hashmap.h"
#include "cutils/math.h"
#include "cutils.h"
#include "stdio.h"

const int MIN_CAPACITY = 16;


static uint32_t getIndex(CTLHashMap map, const void *key, size_t keylen);

struct __CTLHashMap {
    uint32_t size;
    uint32_t capacity;
    float loadFactor;
    const void **hashContainer;
    CTLHashFunction hashFunction;
};

CTLHashMap CTLHashMapCreate(const int initialSize, float loadFactor, CTLHashFunction hashFunction) {
    CTLHashMap map = calloc(1, sizeof(struct __CTLHashMap));

    uint32_t capacity = roundToPow2(initialSize);
    capacity = greaterOf(capacity, MIN_CAPACITY);

    map->hashContainer = calloc(capacity, sizeof(uintptr_t));
    map->capacity = capacity;
    map->loadFactor = loadFactor;
    map->hashFunction = hashFunction;

    return map;
}

void CTLHashMapFree(CTLHashMap map) {
    free(map->hashContainer);
    free(map);
}

#ifdef DEBUG
void CTLHashMapDebugPrint(CTLHashMap map) {
    printf("CTLHashMap %p ", (void *)map);
    printf("capacity: %u ", map->capacity);
    printf("loadFactor: %f ", map->loadFactor);
    printf("hash function: 0x%lx ", (uintptr_t)map->hashFunction);
    printf("size: %d\n", map->size);
}
#endif

int CTLHashMapSize(CTLHashMap map) {
    return map->size;
}

bool CTLHashMapIsEmpty(CTLHashMap map) {
    return map->size == 0;
}

bool CTLHashMapContains(CTLHashMap map, void *key, size_t keylen) {
    return map->hashContainer[getIndex(map, key, keylen)] != NULL;
}

const void *CTLHashMapGet(CTLHashMap map, const void *key, size_t keylen) {
    const uint64_t hash = map->hashFunction(key, keylen);
    const uint32_t index = hash % map->capacity;
    return map->hashContainer[index];
}

const void *CTLHashMapPut(CTLHashMap map, const void* key, size_t keylen, const void *entry) {
    const uint64_t hash = map->hashFunction(key, keylen);
    const uint32_t index = hash % map->capacity;

    if (map->hashContainer[index] == NULL) {
        map->hashContainer[index] = (void *)entry;
        map->size++;
        return (void *)entry;
    }
    return map->hashContainer[index];
}


const void *CTLHashMapRemove(CTLHashMap map, const void *key, size_t keylen) {
    const uint64_t hash = map->hashFunction(key, keylen);
    const uint32_t index = hash % map->capacity;
    const void *result =  map->hashContainer[index];
    if (result != NULL) {
        map->size--;
        map->hashContainer[index] = NULL;
    }
    return result;
}

const void *CTLHashMapPutIfAbsent(CTLHashMap map, const void *key, const size_t keylen, const void *entry) {
    uint32_t index = getIndex(map, key, keylen);
    const void *existing = map->hashContainer[index];
    const void *result = existing;
    if (existing == NULL) {
        map->hashContainer[index] = entry;
        result = (void*)entry;
    }
    return result;;
}


bool CTLHashMapReplace(CTLHashMap map, const void *key, const size_t keylen, const void *entry) {
    uint32_t index = getIndex(map, key, keylen);
    const void * existing = map->hashContainer[index];
    bool wasReplaced = existing != NULL;
    map->hashContainer[index] = entry;
    return wasReplaced;
}

void CTLHashMapKeys(CTLHashMap map, void** keys) {
    NOT_IMPLEMENTED;
    UNUSED_PARAMETER(keys);
    UNUSED_PARAMETER(map);
}

void CTLHashMapClear(CTLHashMap map) {
    for (size_t i = 0; i < map->capacity; i++) {
        map->hashContainer[i] = NULL;
    }
    map->size = 0;
}

static uint32_t getIndex(CTLHashMap map, const void *key, size_t keylen) {
    const uint64_t hash = map->hashFunction(key, keylen);
    return hash % map->capacity;
}
