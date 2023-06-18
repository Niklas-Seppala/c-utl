#include "ctl/iterator.h"

#include "ctl/error.h"

struct iterableNode {
    const struct iterableNode *next;
    const void *value;
};

struct iterableSpan {
    const void *array;
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
    NOT_NULL(iterator);
    NOT_NULL(iterator->node);
    CTLIterable currentNode = iterator->node;
    iterator->node = (void *)iterator->node->next;
    iterator->index++;
    return currentNode->value;
}
inline bool CTLIteratorHasNext(CTLIterator iterator) {
    return iterator->node != NULL;
}