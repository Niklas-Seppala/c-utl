#if !defined(CTL_ITERATOR_H)
#define CTL_ITERATOR_H
#include "ctl.h"

typedef struct iterableNode {
    struct iterableNode *next;
    void *value;
} *CTLIterableNode;

typedef struct iterableArray *CTLIterableArray;
typedef union __CTLIterator *CTLIterator;

/**
 * @brief
 *
 * @param iterator
 * @return void*
 */
void *CTLIteratorNext(CTLIterator iterator);

/**
 * @brief
 *
 * @param iterator
 * @return true
 * @return false
 */
bool CTLIteratorHasNext(CTLIterator iterator);

/**
 * @brief
 *
 * @param firstNode
 * @return CTLIterator
 */
CTLIterator CTLIteratorAllocateNodeIterator(CTLIterableNode firstNode);

/**
 * @brief
 *
 * @param array
 * @param size
 * @return CTLIterator
 */
CTLIterator CTLIteratorAllocateArrayIterator(void **array, size_t size);

/**
 * @brief
 *
 * @param iterator
 */
void CTLIteratorFree(CTLIterator *iterator);

/**
 * @brief
 *
 * @param iterator
 * @return ssize_t
 */
ssize_t CTLIteratorGetIndex(CTLIterator iterator);

/**
 * @brief
 *
 * @param indexDelta
 */
void CTLIteratorUnderlyingChanged(CTLIterator, size_t indexDelta);

/**
 * @brief
 *
 * @param iterator
 */
void CTLIteratorResetHead(CTLIterator iterator);
#endif  // CTL_ITERATOR_H
