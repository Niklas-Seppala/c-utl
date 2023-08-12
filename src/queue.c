#include "ctl/queue.h"

inline CTLQueue CTLQueueCreate() { return CTLLinkedListCreate(); }

inline void CTLQueueFree(CTLQueue *queue) { CTLLinkedListFree(queue); }

inline bool CTLQueueIsEmpty(const CTLQueue queue) {
    return CTLLinkedListIsEmpty(queue);
}

inline void *CTLQueuePeek(const CTLQueue queue) {
    return CTLLinkedListGetFirst(queue);
}

inline void *CTLQueuePoll(CTLQueue queue) {
    return CTLLinkedListRemoveFirst(queue);
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