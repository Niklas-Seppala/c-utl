#include "cutils/iterator.h"
#include "cutils/error.h"

struct iterableNode {
    const struct iterableNode *next;
    const void *value;
};

struct __CTLIterator {
    CTLIterable node;
    size_t index;
};

CTLIterator CTLIteratorAllocateNodeIterator(CTLIterable firstNode) {
    NOT_NULL(firstNode);
    CTLIterator iterator = calloc(sizeof(struct __CTLIterator), 1);
    iterator->node = firstNode;
    iterator->index = -1;
    return iterator;
}

void CTLIteratorResetHead(CTLIterator iterator, CTLIterable iterable) {
    iterator->node = iterable;
    iterator->index = -1;
}

void CTLIteratorFreeNodeIterator(CTLIterator *iterator) {
    NOT_NULL(iterator);
    free((void *)*iterator);
    *iterator = NULL;
}

inline ssize_t CTLIteratorGetIndex(CTLIterator iterator) {
    NOT_NULL(iterator);
    return iterator->index;
}

inline ssize_t CTLIteratorRemovedCurrent(CTLIterator iter) {
    return --iter->index;
}

const void *CTLIteratorNext(CTLIterator iterator) {
    CTLIterable n = iterator->node;
    iterator->node = (void *)iterator->node->next;
    iterator->index++;
    return n->value;
}
inline bool CTLIteratorHasNext(CTLIterator iterator) {
    return iterator->node != NULL;
}