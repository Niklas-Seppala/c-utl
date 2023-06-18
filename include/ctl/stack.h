#if !defined(CTL_STACK_H)
#define CTL_STACK_H
#include "ctl.h"
#include "ctl/compare.h"
#include "ctl/linkedlist.h"

typedef CTLLinkedList CTLStack;

/**
 * @brief
 *
 * @param stack
 * @return true
 * @return false
 */
bool CTLStackIsEmpty(const CTLStack stack);

/**
 * @brief
 *
 * @param stack
 * @return void*
 */
const void *CTLStackPeek(const CTLStack stack);

/**
 * @brief
 *
 * @param stack
 * @return void*
 */
const void *CTLStackPop(CTLStack stack);

/**
 * @brief
 *
 * @param stack
 * @param entry
 */
void CTLStackPush(CTLStack stack, const void *entry);

/**
 * @brief
 *
 * @param stack
 * @param entry
 * @param entryComparer
 * @return int64_t
 */
int64_t CTLStackFind(CTLStack stack, const void *entry,
                     CTLCompareFunction entryComparer);

CTLIterator CTLStackIteratorAllocate(const CTLStack stack);

#endif  // CTL_STACK_H
