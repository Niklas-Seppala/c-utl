#include "ctl/hashset.h"

inline CTLHashSet CTLHashSetCreate(int initialSize, float loadFactor,
                                   CTLHashFunction hashFunction,
                                   CTLCompareFunction keyCompareFunction) {
    return CTLHashMapCreate(initialSize, loadFactor, hashFunction,
                            keyCompareFunction, NULL);
}

inline void CTLHashSetFree(CTLHashSet *set) { CTLHashMapFree(set); }

inline bool CTLHashSetAdd(CTLHashSet set, const void *entry, size_t size) {
    if (CTLHashMapContainsKey(set, entry, size)) {
        return false;
    } else {
        CTLHashMapPut(set, entry, size, NULL);
        return true;
    }
}

inline bool CTLHashSetRemove(CTLHashSet set, const void *entry, size_t size) {
    if (CTLHashMapContainsKey(set, entry, size)) {
        CTLHashMapRemove(set, entry, size);
        return true;
    } else {
        return false;
    }
}

inline void CTLHashSetClear(CTLHashSet set) { CTLHashMapClear(set); }

inline bool CTLHashSetContains(CTLHashSet set, const void *entry, size_t size) {
    return CTLHashMapContainsKey(set, entry, size);
}

inline bool CTLHashSetIsEmpty(CTLHashSet set) { return CTLHashMapIsEmpty(set); }

inline size_t CTLHashSetSize(CTLHashSet set) { return CTLHashMapSize(set); }