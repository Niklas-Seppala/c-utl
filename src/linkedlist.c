#include "ctl/linkedlist.h"

#include <stdarg.h>

#include "ctl.h"
#include "ctl/error.h"

#define EMPTY 0
#define INVALID -1

struct __CTLLinkedList {
    struct CTLLinkedListNode *head;
    struct CTLLinkedListNode *tail;
    size_t size;
};

#ifdef DEBUG
void CTLLinkedListDebugPrint(CTLLinkedList list) {
    struct CTLLinkedListNode *head = list->head;
    printf("----------------------------------------------------\n");
    printf("LinkedList %p size: %ld\n", (void *)list, list->size);
    printf("head: %p tail: %p\n", (void *)list->head, (void *)list->tail);
    printf("entries: ");
    while (head != NULL) {
        printf("%p", (void *)head->entry);
        head = head->next;
        if (head != NULL) printf(" -> ");
    }
    printf("\n");
    printf("----------------------------------------------------\n");
}

void CTLLinkedListDebugPrintf(CTLLinkedList list, CTLObjectToString provider) {
    struct CTLLinkedListNode *head = list->head;
    printf("----------------------------------------------------\n");
    printf("LinkedList %p size: %ld\n", (void *)list, list->size);
    printf("head: %p tail: %p\n", (void *)list->head, (void *)list->tail);
    printf("entries: ");
    while (head != NULL) {
        printf("(%s)", provider(head->entry));
        head = head->next;
        if (head != NULL) printf(" -> ");
    }
    printf("\n");
    printf("----------------------------------------------------\n");
}
#endif

inline static void __CTLUnlinkAndFreeNode(CTLLinkedList list,
                                          struct CTLLinkedListNode **node,
                                          struct CTLLinkedListNode *prev);
inline static void __CTLAttachTo(struct CTLLinkedListNode *attach,
                                 struct CTLLinkedListNode *node);
inline static struct CTLLinkedListNode *__CTLAllocateEntryNode(
    const void *entry);

CTLLinkedList CTLLinkedListCreate(void) {
    CTLLinkedList list = calloc(1, sizeof(struct __CTLLinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = EMPTY;
    return list;
}

void CTLLinkedListFree(CTLLinkedList *list) {
    NOT_NULL(list);
    struct CTLLinkedListNode *head = (*list)->head;
    while (head != NULL) {
        struct CTLLinkedListNode *next = head->next;
        free(head);
        head = next;
    }
    free((void *)*list);
    *list = NULL;
}

CTLLinkedList CTLLinkedListCopy(CTLLinkedList list) {
    NOT_NULL(list);
    CTLLinkedList copy = CTLLinkedListCreate();
    if (CTLLinkedListIsEmpty(list)) {
        return copy;
    }

    struct CTLLinkedListNode *head = list->head;
    while (head != NULL) {
        CTLLinkedListAdd(copy, head->entry);
        head = head->next;
    }

    return copy;
}

int CTLLinkedListSize(CTLLinkedList list) {
    NOT_NULL(list);
    return list->size;
}

bool CTLLinkedListIsEmpty(CTLLinkedList list) {
    NOT_NULL(list);
    return list->size == EMPTY;
}

const void *CTLLinkedListGet(CTLLinkedList list, size_t atIndex) {
    NOT_NULL(list);
    size_t slot = EMPTY;

    if (CTLLinkedListIsEmpty(list)) {
        return NULL;
    }

    struct CTLLinkedListNode *head = list->head;
    while (head != NULL) {
        if ((slot++ == atIndex)) {
            const void *entry = head->entry;
            return entry;
        }
        head = head->next;
    }
    return NULL;
}

const void *CTLLinkedListRemoveAt(CTLLinkedList list, size_t atIndex) {
    NOT_NULL(list);
    size_t slot = EMPTY;

    if (CTLLinkedListIsEmpty(list)) {
        return NULL;
    }

    struct CTLLinkedListNode *head = list->head;
    struct CTLLinkedListNode *prev = NULL;
    while (head != NULL) {
        if ((slot++ == atIndex)) {
            const void *entry = head->entry;
            __CTLUnlinkAndFreeNode(list, &head, prev);
            return entry;
        }
        prev = head;
        head = head->next;
    }
    return NULL;
}

const void *CTLLinkedListRemoveFirst(CTLLinkedList list) {
    NOT_NULL(list);
    if (CTLLinkedListIsEmpty(list)) {
        return NULL;
    }
    struct CTLLinkedListNode *head = list->head;
    list->head = head->next;
    if (list->tail == head) {
        list->tail = head->next;
    }
    const void *result = head->entry;
    free(head);
    head = NULL;
    list->size--;
    return result;
}

const void *CTLLinkedListRemoveLast(CTLLinkedList list) {
    NOT_NULL(list);
    struct CTLLinkedListNode *head = list->head;
    struct CTLLinkedListNode *prev = NULL;
    while (head->next != NULL) {
        prev = head;
        head = head->next;
    }
    const void *result = head->entry;
    __CTLUnlinkAndFreeNode(list, &head, prev);
    return result;
}

size_t CTLLinkedListInsertAt(CTLLinkedList list, const void *entry,
                             size_t atIndex) {
    NOT_NULL(list);

    size_t slot = EMPTY;
    struct CTLLinkedListNode *node = __CTLAllocateEntryNode(entry);

    if (CTLLinkedListIsEmpty(list)) {
        list->head = node;
        list->tail = node;
        list->size++;
        return slot;
    }

    struct CTLLinkedListNode *head = list->head;
    struct CTLLinkedListNode *prev = NULL;
    while (head != NULL) {
        if ((slot == atIndex)) {
            if (prev == NULL) {
                node->next = list->head;
                list->head = node;
            } else {
                __CTLAttachTo(head, node);
            }
            list->size++;
            return slot;
        }
        prev = head;
        head = head->next;
        slot++;
    }

    __CTLAttachTo(prev, node);
    list->size++;
    return slot;
}

inline CTLIterableNode CTLLinkedListRawIterator(CTLLinkedList list) {
    NOT_NULL(list);
    return (CTLIterableNode)list->head;
}

inline CTLIterator CTLLinkedListIteratorAllocate(CTLLinkedList list) {
    return CTLIteratorAllocateNodeIterator((CTLIterableNode)list->head);
}

void CTLLinkedListAdd(CTLLinkedList list, const void *entry) {
    NOT_NULL(list);
    struct CTLLinkedListNode *node = __CTLAllocateEntryNode(entry);

    if (CTLLinkedListIsEmpty(list)) {
        list->head = node;
        list->tail = node;
    } else {
        __CTLAttachTo(list->tail, node);
        list->tail = node;
    }

    list->size++;
}

void CTLLinkedListAddAll(CTLLinkedList list, size_t count, ...) {
    NOT_NULL(list);
    va_list args;
    va_start(args, count);
    for (size_t i = 0; i < count; i++) {
        CTLLinkedListAdd(list, va_arg(args, void *));
    }
    va_end(args);
}

void CTLLinkedListAddFirst(CTLLinkedList list, const void *entry) {
    NOT_NULL(list);
    struct CTLLinkedListNode *node = __CTLAllocateEntryNode(entry);

    if (CTLLinkedListIsEmpty(list)) {
        list->head = node;
        list->tail = node;
    } else {
        struct CTLLinkedListNode *head = list->head;
        node->next = head;
        list->head = node;
    }
    list->size++;
}

CTLLinkedList CTLLinkedListFilter(CTLLinkedList list, CTLpredicate filter) {
    NOT_NULL(list, filter);
    struct CTLLinkedListNode *head = list->head;
    struct CTLLinkedListNode *prev = NULL;
    while (head != NULL) {
        struct CTLLinkedListNode *next = head->next;
        if (!filter(head->entry)) {
            __CTLUnlinkAndFreeNode(list, &head, prev);
            // If multiple filters happen in a row, last valid
            // node is still in prev.
            head = prev;
        }
        prev = head;
        head = next;
    }
    return list;
}

void CTLLinkedListClear(CTLLinkedList list) {
    NOT_NULL(list);
    if (CTLLinkedListIsEmpty(list)) {
        return;
    }

    struct CTLLinkedListNode *head = list->head;
    while (head != NULL) {
        struct CTLLinkedListNode *next = head->next;
        free(head);
        head = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = EMPTY;
}

const void *CTLLinkedListGetFirst(CTLLinkedList list) {
    NOT_NULL(list);
    if (CTLLinkedListIsEmpty(list)) {
        return NULL;
    } else {
        NOT_NULL(list->head);
        return list->head->entry;
    }
}

const void *CTLLinkedListGetLast(CTLLinkedList list) {
    NOT_NULL(list);
    if (CTLLinkedListIsEmpty(list)) {
        return NULL;
    } else {
        return list->tail->entry;
    }
}

const void *CTLLinkedListFind(CTLLinkedList list, const void *entry,
                              CTLCompareFunction compareFunction) {
    NOT_NULL(list, compareFunction);
    if (CTLLinkedListIsEmpty(list)) {
        return NULL;
    }
    struct CTLLinkedListNode *head = list->head;
    while (head != NULL) {
        if (compareFunction(head->entry, entry) == EQ) {
            return head->entry;
        }
        head = head->next;
    }
    return NULL;
}

CTLLinkedList CTLLinkedListReverse(CTLLinkedList list) {
    NOT_NULL(list);
    struct CTLLinkedListNode *prev = NULL;
    struct CTLLinkedListNode *head = list->head;
    struct CTLLinkedListNode *newTail = head;
    struct CTLLinkedListNode *next = NULL;
    while (head != NULL) {
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    list->head = prev;
    list->tail = newTail;
    return list;
}

const void *CTLLinkedListRemove(CTLLinkedList list, const void *entry,
                                CTLCompareFunction compareFunction) {
    NOT_NULL(list, compareFunction);
    struct CTLLinkedListNode *head = list->head;
    struct CTLLinkedListNode *prev = NULL;
    while (head != NULL) {
        if (compareFunction(head->entry, entry) == EQ) {
            const void *value = head->entry;
            __CTLUnlinkAndFreeNode(list, &head, prev);
            return value;
        }
        prev = head;
        head = head->next;
    }
    return NULL;
}

int64_t CTLLinkedListIndexOf(CTLLinkedList list, const void *entry,
                             CTLCompareFunction compareFuntion) {
    NOT_NULL(list, compareFuntion);
    size_t index = 0;
    struct CTLLinkedListNode *node = list->head;
    while (node != NULL) {
        if (compareFuntion(entry, node->entry) == EQ) {
            return index;
        }
        node = node->next;
        index++;
    }
    return -1;
}

bool CTLLinkedListContains(CTLLinkedList list, const void *entry,
                           CTLCompareFunction compareFuntion) {
    NOT_NULL(list, compareFuntion);
    struct CTLLinkedListNode *node = list->head;
    while (node != NULL) {
        if (compareFuntion(entry, node->entry) == EQ) {
            return true;
        }
        node = node->next;
    }
    return false;
}

void CTLLinkedListForEach(CTLLinkedList list, CTLConsumer entryConsumer) {
    NOT_NULL(list, entryConsumer);
    struct CTLLinkedListNode *node = list->head;
    while (node != NULL) {
        entryConsumer(node->entry);
        node = node->next;
    }
}

static inline void __CTLUnlinkAndFreeNode(CTLLinkedList list,
                                          struct CTLLinkedListNode **node,
                                          struct CTLLinkedListNode *prev) {
    NOT_NULL(list, node);

    if (list->head == *node) {
        list->head = (*node)->next;
    }
    if (list->tail == *node) {
        list->tail = prev;
    }
    if (prev != NULL) {
        prev->next = (*node)->next;
    }

    list->size--;
    if (CTLLinkedListIsEmpty(list)) {
        list->tail = NULL;
    }
    free(*node);
    *node = NULL;
}

inline static struct CTLLinkedListNode *__CTLAllocateEntryNode(
    const void *entry) {
    struct CTLLinkedListNode *node =
        calloc(1, sizeof(struct CTLLinkedListNode));
    node->entry = entry;
    return node;
}

inline static void __CTLAttachTo(struct CTLLinkedListNode *attach,
                                 struct CTLLinkedListNode *node) {
    NOT_NULL(attach, node);
    node->next = attach->next;
    attach->next = node;
}
