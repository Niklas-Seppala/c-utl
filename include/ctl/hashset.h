#if !defined(CTL_HASHSET_H)
#define CTL_HASHSET_H
#include "ctl.h"
#include "ctl/hashmap.h"

typedef CTLHashMap CTLHashSet;

/**
 * @brief
 *
 * @param initialSize
 * @param loadFactor
 * @param hashFunction
 * @param keyCompareFunction
 * @return CTLHashSet
 */
CTLHashSet CTLHashSetCreate(int initialSize, float loadFactor,
                            CTLHashFunction hashFunction,
                            CTLCompareFunction keyCompareFunction);

void CTLHashSetFree(CTLHashSet *set);

/**
 * @brief
 *
 * @param set
 * @param entry
 * @param size
 * @return true
 * @return false
 */
bool CTLHashSetAdd(CTLHashSet set, const void *entry, size_t size);

/**
 * @brief
 *
 * @param set
 * @param entry
 * @param size
 * @return true
 * @return false
 */
bool CTLHashSetRemove(CTLHashSet set, const void *entry, size_t size);

/**
 * @brief
 *
 * @param set
 */
void CTLHashSetClear(CTLHashSet set);

/**
 * @brief
 *
 * @param set
 * @param entry
 * @param size
 * @return true
 * @return false
 */
bool CTLHashSetContains(CTLHashSet set, const void *entry, size_t size);

/**
 * @brief
 *
 * @param set
 * @return true
 * @return false
 */
bool CTLHashSetIsEmpty(CTLHashSet set);

/**
 * @brief
 *
 * @param set
 * @return size_t
 */
size_t CTLHashSetSize(CTLHashSet set);

#endif  // CTL_HASHSET_H
