#if !defined(CTL_QUEUE_H)
#define CTL_QUEUE_H
#include "ctl.h"
#include "ctl/compare.h"
#include "ctl/linkedlist.h"

typedef CTLLinkedList CTLQueue;

/**
 * @brief
 *
 * @return CTLQueue
 */
CTLQueue CTLQueueCreate();

/**
 * @brief
 *
 * @param queue
 */
void CTLQueueFree(CTLQueue *queue);

/**
 * @brief
 *
 * @param queue
 * @return true
 * @return false
 */
bool CTLQueueIsEmpty(const CTLQueue queue);

/**
 * @brief
 *
 * @param queue
 * @return void*
 */
void *CTLQueuePeek(const CTLQueue queue);

/**
 * @brief
 *
 * @param queue
 * @return void*
 */
void *CTLQueuePoll(CTLQueue queue);

/**
 * @brief
 *
 * @param queue
 * @param entry
 */
void CTLQueuePush(CTLQueue queue, const void *entry);

/**
 * @brief
 *
 * @param queue
 * @param entry
 * @param entryComparer
 * @return int64_t
 */
int64_t CTLQueueFind(CTLQueue queue, const void *entry,
                     CTLCompareFunction entryComparer);

/**
 * @brief
 *
 * @param queue
 * @return CTLIterator
 */
CTLIterator CTLQueueIteratorAllocate(const CTLQueue queue);

#endif  // CTL_QUEUE_H
