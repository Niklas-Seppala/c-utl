#include "ctl/queue.h"

inline bool CTLQueueIsEmpty(const CTLQueue queue) {
    return CTLLinkedListIsEmpty(queue);
}

inline const void *CTLQueuePeek(const CTLQueue queue) {
    return CTLLinkedListGetFirst(queue);
}

inline const void *CTLQueuePoll(CTLQueue stack) {
    return CTLLinkedListRemoveFirst(stack);
}

inline void CTLQueuePush(CTLQueue queue, const void *entry) {
    CTLLinkedListAdd(queue, entry);
}

inline int64_t CTLQueueFind(CTLQueue queue, const void *entry,
                            CTLCompareFunction entryComparer) {
    return CTLLinkedListIndexOf(queue, entry, entryComparer);
}

inline CTLIterator CTLQueueIteratorAllocate(const CTLQueue queue) {
    return CTLLinkedListIteratorAllocate(queue);
}