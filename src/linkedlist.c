#include "cutils/linkedlist.h"

#include "cutils.h"
#include "cutils/error.h"
#include "stdint.h"

#define EMPTY 0

struct __CTLLinkedListNode {
    struct __CTLLinkedListNode *next;
    const void *entry;
};

struct __CTLLinkedList {
    struct __CTLLinkedListNode *head;
    struct __CTLLinkedListNode *tail;
    CTLIterator iter;
    size_t size;
};

#ifdef DEBUG
void CTLLinkedListDebugPrint(CTLLinkedList list) {
    struct __CTLLinkedListNode *head = list->head;
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
    struct __CTLLinkedListNode *head = list->head;
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

inline static void __CTLUnlinkAndFreeNode(CTLLinkedList list, struct __CTLLinkedListNode **node, struct __CTLLinkedListNode *prev);
inline static void __CTLAttachTo(struct __CTLLinkedListNode *attach, struct __CTLLinkedListNode *node);
inline static struct __CTLLinkedListNode *__CTLAllocateEntryNode(const void *entry);

CTLLinkedList CTLLinkedListCreate(void) {
    CTLLinkedList list = calloc(1, sizeof(struct __CTLLinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = EMPTY;
    list->iter = NULL;
    return list;
}

void CTLLinkedListFree(CTLLinkedList *list) {
    NOT_NULL(list);
    struct __CTLLinkedListNode *head = (*list)->head;
    while (head != NULL) {
        struct __CTLLinkedListNode *next = head->next;
        free(head);
        head = next;
    }
    free((void *)(*list)->iter);
    free((void *)*list);
    *list = NULL;
}

CTLLinkedList CTLLinkedListCopy(CTLLinkedList list) {
    NOT_NULL(list);
    CTLLinkedList copy = CTLLinkedListCreate();
    if (CTLLinkedListIsEmpty(list)) {
        return copy;
    }

    struct __CTLLinkedListNode *head = list->head;
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

const void *CTLLinkedListRemoveAt(CTLLinkedList list, size_t atIndex) {
    NOT_NULL(list);
    size_t slot = EMPTY;

    if (CTLLinkedListIsEmpty(list)) {
        return NULL;
    }

    struct __CTLLinkedListNode *head = list->head;
    struct __CTLLinkedListNode *prev = NULL;
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

size_t CTLLinkedListInsert(CTLLinkedList list, const void *entry, size_t atIndex) {
    NOT_NULL(list);
    size_t slot = EMPTY;
    struct __CTLLinkedListNode *node = __CTLAllocateEntryNode(entry);

    if (CTLLinkedListIsEmpty(list)) {
        list->head = node;
        list->tail = node;
        list->size++;
        return slot;
    }

    struct __CTLLinkedListNode *head = list->head;
    while (head != NULL) {
        if ((slot++ == atIndex)) {
            __CTLAttachTo(head, node);
        }
        head = head->next;
    }
    list->size++;
    return slot;
}

void CTLLinkedListIteratorReset(CTLLinkedList list) {
    CTLIteratorResetHead(list->iter, (CTLIterable)list->head);
}

inline CTLIterator CTLLinkedListIterator(CTLLinkedList list) {
    list->iter = CTLIteratorAllocateNodeIterator((CTLIterable)list->head);
    return list->iter;
}

void CTLLinkedListAdd(CTLLinkedList list, const void *entry) {
    NOT_NULL(list);
    struct __CTLLinkedListNode *node = __CTLAllocateEntryNode(entry);

    if (CTLLinkedListIsEmpty(list)) {
        list->head = node;
        list->tail = node;
    } else {
        __CTLAttachTo(list->tail, node);
        list->tail = node;
    }

    list->size++;
}

void CTLLinkedListAddFirst(CTLLinkedList list, const void *entry) {
    NOT_NULL(list);
    struct __CTLLinkedListNode *node = __CTLAllocateEntryNode(entry);

    if (CTLLinkedListIsEmpty(list)) {
        list->head = node;
        list->tail = node;
    } else {
        struct __CTLLinkedListNode *head = list->head;
        node->next = head;
        list->head = node;
    }
    list->size++;
}

CTLLinkedList CTLLinkedListFilter(CTLLinkedList list, CTLpredicate filter) {
    NOT_NULL(list);
    NOT_NULL(filter);
    struct __CTLLinkedListNode *head = list->head;
    struct __CTLLinkedListNode *prev = NULL;
    while (head != NULL) {
        struct __CTLLinkedListNode *next = head->next;
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

    struct __CTLLinkedListNode *head = list->head;
    while (head != NULL) {
        struct __CTLLinkedListNode *next = head->next;
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

const void *CTLLinkedListFind(CTLLinkedList list, const void *entry, CTLCompareFunction compareFunction) {
    NOT_NULL(list);
    NOT_NULL(entry);
    NOT_NULL(compareFunction);
    if (CTLLinkedListIsEmpty(list)) {
        return NULL;
    }
    struct __CTLLinkedListNode *head = list->head;
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
    struct __CTLLinkedListNode *prev = NULL;
    struct __CTLLinkedListNode *head = list->head;
    struct __CTLLinkedListNode *next = NULL;
    while (head != NULL) {
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    list->head = prev;
    return list;
}

const void *CTLLinkedListRemove(CTLLinkedList list, const void *entry, CTLCompareFunction compareFunction) {
    NOT_NULL(list);
    NOT_NULL(compareFunction);
    struct __CTLLinkedListNode *head = list->head;
    struct __CTLLinkedListNode *prev = NULL;
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

int64_t CTLLinkedListIndexOf(CTLLinkedList list, const void *entry, CTLCompareFunction compareFuntion) {
    NOT_NULL(list);
    NOT_NULL(compareFuntion);
    size_t index = 0;
    struct __CTLLinkedListNode *node = list->head;
    while (node != NULL) {
        if (compareFuntion(entry, node->entry) == EQ) {
            return index;
        }
        node = node->next;
        index++;
    }
    return -1;
}

bool CTLLinkedListContains(CTLLinkedList list, const void *entry, CTLCompareFunction compareFuntion) {
    NOT_NULL(list);
    NOT_NULL(compareFuntion);
    struct __CTLLinkedListNode *node = list->head;
    while (node != NULL) {
        if (compareFuntion(entry, node->entry) == EQ) {
            return true;
        }
        node = node->next;
    }
    return false;
}

static inline void __CTLUnlinkAndFreeNode(CTLLinkedList list, struct __CTLLinkedListNode **node, struct __CTLLinkedListNode *prev) {
    NOT_NULL(list);
    NOT_NULL(node);

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

inline static struct __CTLLinkedListNode *__CTLAllocateEntryNode(const void *entry) {
    struct __CTLLinkedListNode *node = calloc(1, sizeof(struct __CTLLinkedListNode));
    node->entry = entry;
    return node;
}

inline static void __CTLAttachTo(struct __CTLLinkedListNode *attach, struct __CTLLinkedListNode *node) {
    NOT_NULL(attach);
    NOT_NULL(node);
    node->next = attach->next;
    attach->next = node;
}
