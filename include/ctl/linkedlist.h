#if !defined(CTL_LINKEDLIST_H)
#define CTL_LINKEDLIST_H
#include "ctl/compare.h"
#include "ctl/iterator.h"
#include "ctl.h"

typedef struct __CTLLinkedList *CTLLinkedList;

#ifdef DEBUG
void CTLLinkedListDebugPrint(CTLLinkedList list);
void CTLLinkedListDebugPrintf(CTLLinkedList list, CTLObjectToString provider);
#endif

CTLLinkedList CTLLinkedListCreate(void);
void CTLLinkedListFree(CTLLinkedList *list);
int CTLLinkedListSize(CTLLinkedList list);
bool CTLLinkedListIsEmpty(CTLLinkedList list);
const void *CTLLinkedListFind(CTLLinkedList list, const void *entry, CTLCompareFunction cmp);
void CTLLinkedListAdd(CTLLinkedList list, const void *entry);
size_t CTLLinkedListInsert(CTLLinkedList list, const void *entry, size_t atIndex);
const void *CTLLinkedListRemoveAt(CTLLinkedList list, size_t atIndex);
void CTLLinkedListAddFirst(CTLLinkedList list, const void *entry);
void CTLLinkedListClear(CTLLinkedList list);
const void *CTLLinkedListGetFirst(CTLLinkedList list);
const void *CTLLinkedListGetLast(CTLLinkedList list);
const void *CTLLinkedListRemove(CTLLinkedList list, const void *entry, CTLCompareFunction compareFunction);
CTLLinkedList CTLLinkedListFilter(CTLLinkedList list, CTLpredicate filter);
CTLLinkedList CTLLinkedListReverse(CTLLinkedList list);
bool CTLLinkedListContains(CTLLinkedList list, const void *entry, CTLCompareFunction comnpareFunction);
int64_t CTLLinkedListIndexOf(CTLLinkedList list, const void *entry, CTLCompareFunction compareFuntion);

CTLLinkedList CTLLinkedListCopy(CTLLinkedList list);

void CTLLinkedListIteratorReset(CTLLinkedList list);
CTLIterator CTLLinkedListIterator(CTLLinkedList list);

#endif  // CTL_LINKEDLIST_H
