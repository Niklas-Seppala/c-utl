#if !defined(CTL_HASHMAP_H)
#define CTL_HASHMAP_H
#include "ctl.h"
#include "ctl/compare.h"
#include "ctl/linkedlist.h"
#include "hashfun.h"

typedef struct __CTLHashMap *CTLHashMap;

typedef void (*CTLEntryConsumer)(const void *key, const void *value);

#ifdef DEBUG
void CTLHashMapDebugPrint(CTLHashMap map);
#endif

/**
 * @brief
 *
 * @param aEntry
 * @param bEntry
 * @return int
 */
int CTLCompareEntryStringKey(const void *aEntry, const void *bEntry);

/**
 * @brief
 *
 * @param entry
 * @param key
 * @return int
 */
int CTLCompareEntryStringValue(const void *entry, const void *key);

/**
 * @brief
 *
 * @param initialSize
 * @param loadFactor
 * @param hashFunction
 * @param keyCompareFunction
 * @param valueCompareFunction
 * @return CTLHashMap
 */
CTLHashMap CTLHashMapCreate(const int initialSize, float loadFactor,
                            CTLHashFunction hashFunction,
                            CTLCompareFunction keyCompareFunction,
                            CTLCompareFunction valueCompareFunction);

/**
 * @brief
 *
 * @param map
 */
void CTLHashMapFree(CTLHashMap *map);

/**
 * @brief
 *
 * @param map
 * @return int
 */
int CTLHashMapSize(CTLHashMap map);

/**
 * @brief
 *
 * @param map
 * @return true
 * @return false
 */
bool CTLHashMapIsEmpty(CTLHashMap map);

/**
 * @brief
 *
 * @param map
 * @param key
 * @param keylen
 * @return true
 * @return false
 */
bool CTLHashMapContainsKey(CTLHashMap map, const void *key,
                           const size_t keylen);

/**
 * @brief
 *
 * @param map
 * @param value
 * @return true
 * @return false
 */
bool CTLHashMapContainsValue(CTLHashMap map, const void *value);

/**
 * @brief
 *
 * @param map
 * @param key
 * @param keylen
 * @param entry
 * @return true
 * @return false
 */
bool CTLHashMapReplace(CTLHashMap map, const void *key, const size_t keylen,
                       const void *entry);

/**
 * @brief
 *
 * @param map
 * @param key
 * @param keylen
 * @return const void*
 */
const void *CTLHashMapGet(CTLHashMap map, const void *key, size_t keylen);

/**
 * @brief
 *
 * @param map
 * @param key
 * @param keylen
 * @param defaultValue
 * @return const void*
 */
const void *CTLHashMapGetOrDefault(CTLHashMap map, const void *key,
                                   size_t keylen, const void *defaultValue);

/**
 * @brief
 *
 * @param map
 * @param key
 * @param keylen
 * @param entry
 * @return const void*
 */
const void *CTLHashMapPut(CTLHashMap map, const void *key, size_t keylen,
                          const void *entry);

/**
 * @brief
 *
 * @param map
 * @param key
 * @param keylen
 * @return const void*
 */
const void *CTLHashMapRemove(CTLHashMap map, const void *key, size_t keylen);

/**
 * @brief
 *
 * @param map
 * @param key
 * @param keylen
 * @param entry
 * @return const void*
 */
const void *CTLHashMapPutIfAbsent(CTLHashMap map, const void *key,
                                  const size_t keylen, const void *entry);

/**
 * @brief
 *
 * @param map
 * @return CTLLinkedList
 */
CTLLinkedList CTLHashMapAllocateKeyList(CTLHashMap map);

/**
 * @brief
 *
 * @param map
 * @return CTLLinkedList
 */
CTLLinkedList CTLHashMapAllocateValueList(CTLHashMap map);

/**
 * @brief
 *
 * @param map
 */
void CTLHashMapClear(CTLHashMap map);

/**
 * @brief
 *
 * @param map
 * @param entryConsumer
 */
void CTLHashMapForEachEntry(CTLHashMap map, CTLEntryConsumer entryConsumer);

#endif  // CTL_HASHMAP_H
