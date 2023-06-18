#include "ctl/stack.h"

inline bool CTLStackIsEmpty(const CTLStack stack) {
    return CTLLinkedListIsEmpty(stack);
}

inline const void *CTLStackPeek(const CTLStack stack) {
    return CTLLinkedListGetLast(stack);
}

inline const void *CTLStackPop(CTLStack stack) {
    return CTLLinkedListRemoveLast(stack);
}

inline void CTLStackPush(CTLStack stack, const void *entry) {
    CTLLinkedListAdd(stack, entry);
}

inline int64_t CTLStackFind(CTLStack stack, const void *entry,
                            CTLCompareFunction entryComparer) {
    return CTLLinkedListIndexOf(stack, entry, entryComparer);
}

inline CTLIterator CTLStackIteratorAllocate(const CTLStack stack) {
    return CTLLinkedListIteratorAllocate(stack);
}