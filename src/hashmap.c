#include "ctl/hashmap.h"

#include "ctl.h"
#include "ctl/compare.h"
#include "ctl/error.h"
#include "ctl/hashfun.h"
#include "ctl/linkedlist.h"
#include "ctl/math.h"
#include "stdio.h"

const int DEFAULT_CAPACITY = 16;

typedef struct kvPair {
    const void *key;
    const void *value;
} KeyValue;

inline int CTLCompareEntryStringKey(const void *entry, const void *key) {
    return CTLCompareString(((KeyValue *)entry)->key, key);
}

inline int CTLCompareEntryStringValue(const void *entry, const void *key) {
    return CTLCompareString(((KeyValue *)entry)->value, key);
}

inline static uint32_t getIndex(const CTLHashMap map, const void *const key,
                                const size_t keylen);
inline static const KeyValue *allocateKeyValue(const void *key,
                                               const void *value);
static void storeKeyValueToBucket(CTLHashMap map, CTLLinkedList bucket,
                                  const int32_t index, const void *key,
                                  const void *entry);
static inline void transferEntryToNewBucket(CTLHashMap map,
                                            const KeyValue *oldKeyValue);
inline static float calcLoad(CTLHashMap map);
static void resizeIfNeeded(CTLHashMap map);

struct __CTLHashMap {
    uint32_t size;
    uint32_t capacity;
    float loadFactor;
    CTLLinkedList *hashContainer;
    CTLHashFunction hashFunction;
    CTLCompareFunction keyCompareFunction;
    CTLCompareFunction valueCompareFunction;
};

CTLHashMap CTLHashMapCreate(const int initialSize, float loadFactor,
                            CTLHashFunction hashFunction,
                            CTLCompareFunction keyCompareFunction,
                            CTLCompareFunction valueCompareFunction) {
    CTLHashMap map = calloc(1, sizeof(struct __CTLHashMap));

    uint32_t capacity = roundToPow2(initialSize);
    capacity = greaterOf(capacity, DEFAULT_CAPACITY);

    map->hashContainer = calloc(capacity, sizeof(uintptr_t));
    map->capacity = capacity;
    map->loadFactor = loadFactor;
    map->hashFunction = hashFunction;
    map->keyCompareFunction = keyCompareFunction;
    map->valueCompareFunction = valueCompareFunction;

    return map;
}

void CTLHashMapFree(CTLHashMap *map) {
    NOT_NULL(map);
    CTLHashMapClear(*map);
    free((*map)->hashContainer);
    free((*map));
    *map = NULL;
}

#ifdef DEBUG
void CTLHashMapDebugPrint(CTLHashMap map) {
    NOT_NULL(map);
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
    NOT_NULL(map);
    return map->size;
}

bool CTLHashMapIsEmpty(CTLHashMap map) {
    NOT_NULL(map);
    return map->size == 0;
}

bool CTLHashMapContainsKey(CTLHashMap map, const void *key, size_t keylen) {
    NOT_NULL(map);
    const uint32_t index = getIndex(map, key, keylen);
    const CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket != NULL) {
        const KeyValue *keyValue =
            CTLLinkedListFind(bucket, key, map->keyCompareFunction);
        if (keyValue != NULL) {
            return true;
        }
    }
    return false;
}

bool CTLHashMapContainsValue(CTLHashMap map, const void *value) {
    NOT_NULL(map);
    for (size_t i = 0; i < map->capacity; i++) {
        CTLLinkedList bucket = map->hashContainer[i];
        if (bucket != NULL) {
            CTLIterator bucketIterator = CTLLinkedListIterator(bucket);
            while (CTLIteratorHasNext(bucketIterator)) {
                const KeyValue *keyValuePair = CTLIteratorNext(bucketIterator);
                if (keyValuePair != NULL) {
                    if (map->valueCompareFunction(keyValuePair, value) == EQ) {
                        return true;
                    }
                }
            }
            CTLLinkedListIteratorReset(bucket);
        }
    }
    return false;
}

const void *CTLHashMapGet(CTLHashMap map, const void *key, size_t keylen) {
    NOT_NULL(map);
    const uint32_t index = getIndex(map, key, keylen);
    const CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket != NULL) {
        const KeyValue *keyValue =
            CTLLinkedListFind(bucket, key, map->keyCompareFunction);
        if (keyValue != NULL) {
            return keyValue->value;
        }
    }
    return NULL;
}

const void *CTLHashMapGetOrDefault(CTLHashMap map, const void *key,
                                   size_t keylen, const void *defaultValue) {
    NOT_NULL(map);
    const uint32_t index = getIndex(map, key, keylen);
    const CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket != NULL) {
        const KeyValue *keyValue =
            CTLLinkedListFind(bucket, key, map->keyCompareFunction);
        if (keyValue != NULL) {
            return keyValue->value;
        }
    }
    return defaultValue;
}

const void *CTLHashMapPut(CTLHashMap map, const void *key, size_t keylen,
                          const void *value) {
    NOT_NULL(map);
    resizeIfNeeded(map);
    const uint32_t index = getIndex(map, key, keylen);
    CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket == NULL) {
        storeKeyValueToBucket(map, bucket, index, key, value);
        return value;
    } else {
        const KeyValue *existing =
            CTLLinkedListRemove(bucket, key, map->keyCompareFunction);
        storeKeyValueToBucket(map, bucket, index, key, value);
        if (existing) {
            const void *existingValue = existing->value;
            free((void *)existing);
            map->size--;
            return existingValue;
        } else {
            return NULL;
        }
    }
}

const void *CTLHashMapRemove(CTLHashMap map, const void *key, size_t keylen) {
    NOT_NULL(map);
    const uint32_t index = getIndex(map, key, keylen);
    const CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket != NULL) {
        const KeyValue *existing =
            CTLLinkedListRemove(bucket, key, map->keyCompareFunction);
        if (existing != NULL) {
            const void *value = existing->value;
            free((void *)existing);
            map->size--;
            return value;
        }
    }
    return NULL;
}

const void *CTLHashMapPutIfAbsent(CTLHashMap map, const void *key,
                                  const size_t keylen, const void *value) {
    NOT_NULL(map);
    resizeIfNeeded(map);
    uint32_t index = getIndex(map, key, keylen);
    CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket == NULL) {
        storeKeyValueToBucket(map, bucket, index, key, value);
        return NULL;
    } else {
        const KeyValue *existing =
            CTLLinkedListFind(bucket, key, map->keyCompareFunction);
        if (existing == NULL) {
            storeKeyValueToBucket(map, bucket, index, key, value);
            return existing;
        } else {
            return existing->value;
        }
    }
}

bool CTLHashMapReplace(CTLHashMap map, const void *key, const size_t keylen,
                       const void *value) {
    NOT_NULL(map);
    uint32_t index = getIndex(map, key, keylen);
    CTLLinkedList bucket = (void *)map->hashContainer[index];
    if (bucket == NULL) {
        storeKeyValueToBucket(map, bucket, index, key, value);
        return false;
    } else {
        const void *existing =
            CTLLinkedListRemove(bucket, key, map->keyCompareFunction);
        storeKeyValueToBucket(map, bucket, index, key, value);
        if (existing) {
            free((void *)existing);
            map->size--;
            return true;
        } else {
            return false;
        }
    }
}

CTLLinkedList CTLHashMapAllocateValueList(CTLHashMap map) {
    NOT_NULL(map);
    CTLLinkedList values = CTLLinkedListCreate();
    for (size_t i = 0; i < map->capacity; i++) {
        CTLLinkedList bucket = (void *)map->hashContainer[i];
        if (bucket != NULL) {
            CTLIterator iterator = CTLLinkedListIterator(bucket);
            while (CTLIteratorHasNext(iterator)) {
                const KeyValue *kv = CTLIteratorNext(iterator);
                CTLLinkedListAdd(values, kv->value);
            }
            CTLLinkedListIteratorReset(bucket);
        }
    }
    return values;
}

CTLLinkedList CTLHashMapAllocateKeyList(CTLHashMap map) {
    NOT_NULL(map);
    CTLLinkedList keys = CTLLinkedListCreate();
    for (size_t i = 0; i < map->capacity; i++) {
        CTLLinkedList bucket = (void *)map->hashContainer[i];
        if (bucket != NULL) {
            CTLIterator iterator = CTLLinkedListIterator(bucket);
            while (CTLIteratorHasNext(iterator)) {
                const KeyValue *kv = CTLIteratorNext(iterator);
                CTLLinkedListAdd(keys, kv->key);
            }
            CTLLinkedListIteratorReset(bucket);
        }
    }
    return keys;
}

void CTLHashMapClear(CTLHashMap map) {
    NOT_NULL(map);
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

void CTLHashMapForEachEntry(CTLHashMap map, CTLEntryConsumer entryConsumer) {
    NOT_NULL(map);
    for (size_t i = 0; i < map->capacity; i++) {
        CTLLinkedList bucket = (void *)map->hashContainer[i];
        if (bucket != NULL) {
            CTLIterator iterator = CTLLinkedListIterator(bucket);
            while (CTLIteratorHasNext(iterator)) {
                const KeyValue *keyValuePair = CTLIteratorNext(iterator);
                if (keyValuePair != NULL) {
                    entryConsumer(keyValuePair->key, keyValuePair->value);
                }
            }
            CTLLinkedListIteratorReset(bucket);
        }
    }
}

inline static uint32_t getIndex(const CTLHashMap map, const void *const key,
                                const size_t keylen) {
    NOT_NULL(map);
    const uint64_t hash = map->hashFunction(key, keylen);
    return hash % map->capacity;
}

static void storeKeyValueToBucket(const CTLHashMap map, CTLLinkedList bucket,
                                  const int32_t index, const void *key,
                                  const void *value) {
    NOT_NULL(map);
    if (bucket == NULL) bucket = CTLLinkedListCreate();
    const KeyValue *keyValue = allocateKeyValue(key, value);
    CTLLinkedListAdd(bucket, keyValue);
    map->hashContainer[index] = bucket;
    map->size++;
}

inline static float calcLoad(CTLHashMap map) {
    NOT_NULL(map);
    return map->size / (float)map->capacity;
}

inline static const KeyValue *allocateKeyValue(const void *key,
                                               const void *value) {
    KeyValue *keyValue = calloc(sizeof(KeyValue), 1);
    keyValue->key = key;
    keyValue->value = value;
    return keyValue;
}

static inline void resizeMap(CTLHashMap map) {
    map->capacity = map->capacity << 1;
    map->hashContainer = calloc(sizeof(uintptr_t), map->capacity);
}

static inline void transferEntryToNewBucket(CTLHashMap map,
                                            const KeyValue *oldKeyValue) {
    // TODO: keylength
    const uint32_t index = getIndex(map, oldKeyValue->key, 0);
    // Get the new bucket, if it was created and rehash collides.
    CTLLinkedList bucket = map->hashContainer[index];
    if (bucket == NULL) {
        bucket = CTLLinkedListCreate();
    }
    CTLLinkedListAdd(bucket, oldKeyValue);
    if (map->hashContainer[index] == NULL) {
        map->hashContainer[index] = bucket;
    }
}

static void resizeIfNeeded(CTLHashMap map) {
    NOT_NULL(map);
    float load = calcLoad(map);
    if (load >= map->loadFactor) {
        const size_t oldCap = map->capacity;
        const CTLLinkedList *oldContainer = map->hashContainer;
        resizeMap(map);
        for (size_t i = 0; i < oldCap; i++) {
            const CTLLinkedList bucket = (CTLLinkedList)oldContainer[i];
            if (bucket != NULL) {
                const KeyValue *entry;
                while ((entry = CTLLinkedListRemoveAt(bucket, 0))) {
                    transferEntryToNewBucket(map, entry);
                }
                CTLLinkedListFree((void *)&bucket);
            }
        }
        free((void *)oldContainer);
    }
}
