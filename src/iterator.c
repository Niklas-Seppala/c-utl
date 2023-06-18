#include "ctl/iterator.h"

#include <assert.h>

#include "ctl/error.h"

/**
 * @brief
 *
 */
enum ctlIterType { ITER_NODE, ITER_ARRAY };

struct iterableNodeChain {
    enum ctlIterType type;
    struct iterableNode *node;
    struct iterableNode *headOfChain;
    size_t index;
};

struct iterableNode {
    struct iterableNode *next;
    void *value;
};

struct iterableArray {
    enum ctlIterType type;
    void **array;
    size_t index;
    size_t size;
};

union __CTLIterator {
    enum ctlIterType type;
    struct iterableNodeChain asNode;
    struct iterableArray asArray;
};

static void *nodeIteratorNext(struct iterableNodeChain *nodeIterator);
static bool nodeIteratorhasNext(struct iterableNodeChain *nodeIterator);

static void *arrayIteratorNext(struct iterableArray *arrayIterator);
static bool arrayIteratorHasNext(struct iterableArray *arrayIterator);

static void arrayIteratorResetHead(struct iterableArray *arrayIterator);
static void nodeIteratorResetHead(struct iterableNodeChain *nodeIterator);

CTLIterator CTLIteratorAllocateArrayIterator(void **array, size_t size) {
    struct iterableArray *arrIter = calloc(sizeof(struct iterableArray), 1);
    arrIter->type = ITER_ARRAY;
    arrIter->array = array;
    arrIter->size = size;
    arrIter->index = 0;
    return (CTLIterator)arrIter;
}

CTLIterator CTLIteratorAllocateNodeIterator(CTLIterableNode firstNode) {
    struct iterableNodeChain *nodeIter =
        calloc(sizeof(struct iterableNodeChain), 1);
    nodeIter->type = ITER_NODE;
    nodeIter->node = firstNode;
    nodeIter->headOfChain = firstNode;
    nodeIter->index = 0;
    return (CTLIterator)nodeIter;
}

void CTLIteratorFree(CTLIterator *iterator) {
    NOT_NULL(iterator);
    free(*iterator);
    *iterator = NULL;
}

bool CTLIteratorHasNext(CTLIterator iterator) {
    NOT_NULL(iterator);
    switch (iterator->type) {
        case ITER_ARRAY:
            return arrayIteratorHasNext(&iterator->asArray);
        case ITER_NODE:
            return nodeIteratorhasNext(&iterator->asNode);
        default:
            return NULL;
    }
}

void *CTLIteratorNext(CTLIterator iterator) {
    NOT_NULL(iterator);
    switch (iterator->type) {
        case ITER_ARRAY:
            return arrayIteratorNext(&iterator->asArray);
        case ITER_NODE:
            return nodeIteratorNext(&iterator->asNode);
        default:
            return NULL;
    }
}

void CTLIteratorResetHead(CTLIterator iterator) {
    NOT_NULL(iterator);
    switch (iterator->type) {
        case ITER_ARRAY:
            arrayIteratorResetHead(&iterator->asArray);
            break;
        case ITER_NODE:
            nodeIteratorResetHead(&iterator->asNode);
            break;
        default:
            break;
    }
}

void CTLIteratorUnderlyingChanged(CTLIterator iterator, size_t indexDelta) {
    switch (iterator->type) {
        case ITER_ARRAY:
            iterator->asArray.index += indexDelta;
            break;
        case ITER_NODE:
            iterator->asNode.index += indexDelta;
            break;
        default:
            break;
    }
}

inline ssize_t CTLIteratorGetIndex(CTLIterator iterator) {
    NOT_NULL(iterator);
    switch (iterator->type) {
        case ITER_ARRAY:
            return iterator->asArray.index;
        case ITER_NODE:
            return iterator->asNode.index;
        default:
            return -1;
    }
}

static inline void *arrayIteratorNext(struct iterableArray *arrayIterator) {
    return arrayIterator->array[arrayIterator->index++];
}

static void *nodeIteratorNext(struct iterableNodeChain *nodeIterator) {
    struct iterableNode *currentNode = nodeIterator->node;
    nodeIterator->node = nodeIterator->node->next;
    nodeIterator->index++;
    return currentNode->value;
}

static inline bool arrayIteratorHasNext(struct iterableArray *arrayIterator) {
    return arrayIterator->index < arrayIterator->size;
}

static inline bool nodeIteratorhasNext(struct iterableNodeChain *nodeIterator) {
    return nodeIterator->node;
}

static void arrayIteratorResetHead(struct iterableArray *arrayIterator) {
    arrayIterator->index = 0;
}

static void nodeIteratorResetHead(struct iterableNodeChain *nodeIterator) {
    nodeIterator->node = nodeIterator->headOfChain;
}