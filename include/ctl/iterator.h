#if !defined(CTL_ITERATOR_H)
#define CTL_ITERATOR_H
#include "ctl.h"

typedef struct iterableNode *CTLIterable;
// typedef struct iterableSpan *CTLIterable;
typedef struct __CTLIterator *CTLIterator;

const void *CTLIteratorNext(CTLIterator iterator);
bool CTLIteratorHasNext(CTLIterator iterator);
CTLIterator CTLIteratorAllocateNodeIterator(CTLIterable firstNode);

// CTLIterator CTLIteratorAllocateArrayIterator(CTLIterable array);

void CTLIteratorFreeNodeIterator(CTLIterator *iterator);
ssize_t CTLIteratorGetIndex(CTLIterator iterator);
ssize_t CTLIteratorRemovedCurrent(CTLIterator iter);
void CTLIteratorResetHead(CTLIterator iterator, CTLIterable iterable);

#endif  // CTL_ITERATOR_H
