#if !defined(CTL_LINKEDLIST_H)
#define CTL_LINKEDLIST_H
#include "stdbool.h"
#include "cutils/compare.h"
#include "stdint.h"

typedef struct __CTLLinkedList *CTLLinkedList;

#ifdef DEBUG
void CTLLinkedListDebugPrint(CTLLinkedList list);
#endif

CTLLinkedList CTLLinkedListCreate();
void CTLLinkedListFree(CTLLinkedList *list);
int CTLLinkedListSize(CTLLinkedList list);
bool CTLLinkedListIsEmpty(CTLLinkedList list);
const void *CTLLinkedListFind(CTLLinkedList list, CTLpredicate cmp);
void CTLLinkedListAdd(CTLLinkedList list, const void *entry);
void CTLLinkedListAddFirst(CTLLinkedList list, const void *entry);
void CTLLinkedListClear(CTLLinkedList list);
const void *CTLLinkedListGetFirst(CTLLinkedList list);
const void *CTLLinkedListGetLast(CTLLinkedList list);
void CTLLinkedListRemove(CTLLinkedList list, const void *entry, CTLCompareFunction comnpareFunction);
CTLLinkedList CTLLinkedListFilter(CTLLinkedList list, CTLpredicate filter);
CTLLinkedList CTLLinkedListReverse(CTLLinkedList list);
bool CTLLinkedListContains(CTLLinkedList list, const void *entry, CTLCompareFunction comnpareFunction);
int64_t CTLLinkedListIndexOf(CTLLinkedList list, const void *entry, CTLCompareFunction compareFuntion);

#endif // CTL_LINKEDLIST_H
