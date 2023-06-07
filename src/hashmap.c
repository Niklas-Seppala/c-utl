#include "ctl/hashmap.h"
#include "ctl/compare.h"
#include "ctl/hashfun.h"
#include "ctl/linkedlist.h"
#include "ctl/math.h"
#include "ctl.h"
#include "stdio.h"

const int MIN_CAPACITY = 16;

typedef struct kv {
    const void *key;
    const void *value;
} KeyValue;

inline int CTLCompareStringKey(const void *entry, const void *key) {
    return CTLCompareString(((KeyValue *)entry)->key, key);
}

inline static uint32_t getIndex(CTLHashMap map, const void *key, size_t keylen);
inline static const KeyValue *allocateKeyValue(const void *key, const void *value);
static void storeKeyValueToBucket(CTLHashMap map, CTLLinkedList bucket, const int32_t index, const void *key, const void *entry) ;
inline static float calcLoad(CTLHashMap map);


struct __CTLHashMap {
    uint32_t size;
    uint32_t capacity;
    float loadFactor;
    const void **hashContainer;
    CTLHashFunction hashFunction;
    CTLCompareFunction keyCompareFunction;
};

CTLHashMap CTLHashMapCreate(const int initialSize, float loadFactor, CTLHashFunction hashFunction, CTLCompareFunction keyCompareFunction) {
    CTLHashMap map = calloc(1, sizeof(struct __CTLHashMap));

    uint32_t capacity = roundToPow2(initialSize);
    capacity = greaterOf(capacity, MIN_CAPACITY);

    map->hashContainer = calloc(capacity, sizeof(uintptr_t));
    map->capacity = capacity;
    map->loadFactor = loadFactor;
    map->hashFunction = hashFunction;
    map->keyCompareFunction = keyCompareFunction;

    return map;
}

void CTLHashMapFree(CTLHashMap map) {
    free(map->hashContainer);
    free(map);
}

#ifdef DEBUG
void CTLHashMapDebugPrint(CTLHashMap map) {
    int bucketCount = 0;
    for (size_t i = 0; i < map->capacity; i++) {
        const CTLLinkedList bucket = (void *)map->hashContainer[i];
        if (bucket != NULL) {
            bucketCount++;
        }
    }
    float load = calcLoad(map);
    printf("CTLHashMap %p ", (void *)map);
    printf("capacity: %u ", map->capacity);
    printf("loadFactor: %f ", map->loadFactor);
    printf("bucket count: %d ", bucketCount);
    printf("current load: %f ", load);
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
    const uint32_t index = getIndex(map, key, keylen);
    const CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket != NULL) {
        const KeyValue *keyValue = CTLLinkedListFind(bucket, key, map->keyCompareFunction);
        if (keyValue != NULL) {
            return keyValue->value;
        }
    }
    return NULL;
}

const void *CTLHashMapPut(CTLHashMap map, const void *key, size_t keylen, const void *value) {
    if (calcLoad(map) > map->loadFactor) {
        const size_t newCap = map->capacity << 1;
        map->hashContainer = calloc(sizeof(uintptr_t), newCap);
        TODO("IMPLEMEN");
        for (size_t i = 0; i < map->capacity; i++) {
            CTLLinkedList bucket = (void *)map->hashContainer[i];
            if (bucket != NULL) {
                const KeyValue *keyValuePair;
                while ((keyValuePair = CTLLinkedListRemoveAt(bucket, 0))) {
                    
                }
            }
        }
    }

    const uint32_t index = getIndex(map, key, keylen);
    CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket == NULL) {
        storeKeyValueToBucket(map, bucket, index, key, value);
        return value;
    } else {
        const KeyValue *existing = CTLLinkedListRemove(bucket, key, map->keyCompareFunction);
        storeKeyValueToBucket(map, bucket, index, key, value);
        if (existing) {
            return existing->value;
        } else {
            return NULL;
        }
    }
}

const void *CTLHashMapRemove(CTLHashMap map, const void *key, size_t keylen) {
    const uint32_t index = getIndex(map, key, keylen);
    const CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket != NULL) {
        const KeyValue *existing = CTLLinkedListRemove(bucket, key, map->keyCompareFunction);
        if (existing != NULL) {
            const void *value = existing->value;
            free((void *)existing);
            map->size--;
            return value;
        }
    }
    return NULL;
}

const void *CTLHashMapPutIfAbsent(CTLHashMap map, const void *key, const size_t keylen, const void *value) {
    uint32_t index = getIndex(map, key, keylen);
    CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket == NULL) {
        storeKeyValueToBucket(map, bucket, index, key, value);
        return value;
    } else {
        const KeyValue *existing = CTLLinkedListFind(bucket, key, map->keyCompareFunction);
        if (existing == NULL) {
            storeKeyValueToBucket(map, bucket, index, key, value);
            return existing;
        } else {
            return existing->value;
        }
    }
}


bool CTLHashMapReplace(CTLHashMap map, const void *key, const size_t keylen, const void *value) {
    uint32_t index = getIndex(map, key, keylen);
    CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket == NULL) {
        storeKeyValueToBucket(map, bucket, index, key, value);
        return false;
    } else {
        const void *existing = CTLLinkedListRemove(bucket, key, map->keyCompareFunction);
        storeKeyValueToBucket(map, bucket, index, key, value);
        if (existing) {
            free((void *)existing);
            return true;
        } else {
            return false;
        }
    }
}

CTLLinkedList CTLHashMapAllocateKeyList(CTLHashMap map) {
    CTLLinkedList keySet = CTLLinkedListCreate();
    for (size_t i = 0; i < map->capacity; i++) {
        CTLLinkedList bucket = (void *)map->hashContainer[i];
        if (bucket != NULL) {
            CTLIterator iterator = CTLLinkedListIterator(bucket);
            while (CTLIteratorHasNext(iterator)) {
                const KeyValue *kv = CTLIteratorNext(iterator);
                CTLLinkedListAdd(keySet, kv->key);
            }
        }
    }
    return keySet;
}

void CTLHashMapClear(CTLHashMap map) {
    for (size_t i = 0; i < map->capacity; i++) {
        CTLLinkedList bucket = (void *)map->hashContainer[i];
        if (bucket != NULL) {
            const KeyValue *keyValuePair;
            while ((keyValuePair = CTLLinkedListRemoveAt(bucket, 0))) {
                free((void *)keyValuePair);
            }
            CTLLinkedListFree(&bucket);
        }
        map->hashContainer[i] = NULL;
    }
    map->size = 0;
}

inline static uint32_t getIndex(CTLHashMap map, const void *key, size_t keylen) {
    const uint64_t hash = map->hashFunction(key, keylen);
    return hash % map->capacity;
}

static void storeKeyValueToBucket(CTLHashMap map, CTLLinkedList bucket, const int32_t index, const void *key, const void *value) {
    if (bucket == NULL) bucket = CTLLinkedListCreate();
    const KeyValue *keyValue = allocateKeyValue(key, value);
    CTLLinkedListAdd(bucket, keyValue);
    map->hashContainer[index] = bucket;
    map->size++;
}

inline static float calcLoad(CTLHashMap map) {
    return map->size / (float)map->capacity;
}

inline static const KeyValue *allocateKeyValue(const void *key, const void *value) {
    KeyValue *keyValue = calloc(sizeof(KeyValue), 1);
    keyValue->key = key;
    keyValue->value = value;
    return keyValue;
}
