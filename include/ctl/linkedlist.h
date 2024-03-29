#if !defined(CTL_LINKEDLIST_H)
#define CTL_LINKEDLIST_H
#include "ctl.h"
#include "ctl/compare.h"
#include "ctl/iterator.h"

struct CTLLinkedListNode {
    struct CTLLinkedListNode *next;
    void *entry;
};

typedef struct __CTLLinkedList *CTLLinkedList;

#ifdef DEBUG
void CTLLinkedListDebugPrint(CTLLinkedList list);
void CTLLinkedListDebugPrintf(CTLLinkedList list, CTLObjectToString provider);
#endif

/**
 * @brief
 *
 * @return CTLLinkedList
 */
CTLLinkedList CTLLinkedListCreate(void);

/**
 * @brief
 *
 * @param list
 */
void CTLLinkedListFree(CTLLinkedList *list);

/**
 * @brief
 *
 * @param list
 * @return int
 */
int CTLLinkedListSize(CTLLinkedList list);

/**
 * @brief
 *
 * @param list
 * @return true
 * @return false
 */
bool CTLLinkedListIsEmpty(CTLLinkedList list);

/**
 * @brief
 *
 * @param list
 * @param entry
 * @param cmp
 * @return void*
 */
void *CTLLinkedListFind(CTLLinkedList list, const void *entry,
                        CTLCompareFunction cmp);

/**
 * @brief
 *
 * @param list
 * @param entry
 */
void CTLLinkedListAdd(CTLLinkedList list, const void *entry);

/**
 * @brief
 *
 * @param list
 * @param count
 * @param ...
 */
void CTLLinkedListAddAll(CTLLinkedList list, size_t count, ...);

/**
 * @brief
 *
 * @param list
 * @param entry
 * @param atIndex
 * @return size_t
 */
size_t CTLLinkedListInsertAt(CTLLinkedList list, const void *entry,
                             size_t atIndex);

/**
 * @brief
 *
 * @param list
 * @param atIndex
 * @return void*
 */
void *CTLLinkedListGet(CTLLinkedList list, size_t atIndex);

/**
 * @brief
 *
 * @param list
 * @param atIndex
 * @return void*
 */
void *CTLLinkedListRemoveAt(CTLLinkedList list, size_t atIndex);

/**
 * @brief
 *
 * @param list
 * @param entry
 */
void CTLLinkedListAddFirst(CTLLinkedList list, const void *entry);

/**
 * @brief
 *
 * @param list
 */
void CTLLinkedListClear(CTLLinkedList list);

/**
 * @brief
 *
 * @param list
 * @return void*
 */
void *CTLLinkedListGetFirst(CTLLinkedList list);

/**
 * @brief
 *
 * @param list
 * @return void*
 */
void *CTLLinkedListGetLast(CTLLinkedList list);

/**
 * @brief
 *
 * @param list
 * @param entry
 * @param compareFunction
 * @return void*
 */
void *CTLLinkedListRemove(CTLLinkedList list, const void *entry,
                          CTLCompareFunction compareFunction);

/**
 * @brief
 *
 * @param list
 * @param entry
 * @param compareFunction
 * @return void*
 */
void *CTLLinkedListRemoveLast(CTLLinkedList list);

/**
 * @brief
 *
 * @param list
 * @param entry
 * @param compareFunction
 * @return void*
 */
void *CTLLinkedListRemoveFirst(CTLLinkedList list);

/**
 * @brief
 *
 * @param list
 * @param filter
 * @return CTLLinkedList
 */
CTLLinkedList CTLLinkedListFilter(CTLLinkedList list, CTLpredicate filter);

/**
 * @brief
 *
 * @param list
 * @return CTLLinkedList
 */
CTLLinkedList CTLLinkedListReverse(CTLLinkedList list);

/**
 * @brief
 *
 * @param list
 * @param entry
 * @param comnpareFunction
 * @return true
 * @return false
 */
bool CTLLinkedListContains(CTLLinkedList list, const void *entry,
                           CTLCompareFunction comnpareFunction);

/**
 * @brief
 *
 * @param list
 * @param entry
 * @param compareFuntion
 * @return int64_t
 */
int64_t CTLLinkedListIndexOf(CTLLinkedList list, const void *entry,
                             CTLCompareFunction compareFuntion);

/**
 * @brief
 *
 * @param list
 * @return CTLLinkedList
 */
CTLLinkedList CTLLinkedListCopy(CTLLinkedList list);

/**
 * @brief
 *
 * @param list
 * @param enntryConsumer
 */
void CTLLinkedListForEach(CTLLinkedList list, CTLConsumer enntryConsumer);

/**
 * @brief
 *
 * @param list
 * @return CTLIterableNode
 */
CTLIterableNode CTLLinkedListRawIterator(CTLLinkedList list);

/**
 * @brief
 *
 * @param list
 * @return CTLIterator
 */
CTLIterator CTLLinkedListIteratorAllocate(CTLLinkedList list);

#endif  // CTL_LINKEDLIST_H
