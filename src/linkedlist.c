#include "cutils/linkedlist.h"
#include "cutils.h"
#include "stdint.h"

#define EMPTY 0

struct __CTLLinkedListNode {
    struct __CTLLinkedListNode *next;
    struct __CTLLinkedListNode *prev;
    const void *entry;
};

struct __CTLLinkedList {
    size_t size;
    struct __CTLLinkedListNode *head;
    struct __CTLLinkedListNode *tail;
};

#ifdef DEBUG
void CTLLinkedListDebugPrint(CTLLinkedList list) {
    struct __CTLLinkedListNode *head = list->head;
    printf("LinkedList %p size: %ld\n", (void *)list, list->size);
    while (head != NULL) {
        printf("%p ", (void *)head);
        head = head->next;
    }
}
#endif

inline static void *allocateNode() { return calloc(1, sizeof(struct __CTLLinkedListNode)); }

CTLLinkedList CTLLinkedListCreate() {
    CTLLinkedList list = calloc(1, sizeof(struct __CTLLinkedList));
    list->head = NULL;
    list->tail = NULL;
    list->size = EMPTY;
    return list;
}

void CTLLinkedListFree(CTLLinkedList *list) {
    struct __CTLLinkedListNode *node = (*list)->head;
    while (node != NULL) {
        struct __CTLLinkedListNode *next = node->next;
        free(node);
        node = next;
    }
    free(*list);
    *list = NULL;
}

int CTLLinkedListSize(CTLLinkedList list) {
    return list->size;
}

bool CTLLinkedListIsEmpty(CTLLinkedList list) {
    return list->size == EMPTY;
}

void CTLLinkedListAdd(CTLLinkedList list, const void *entry) {
    struct __CTLLinkedListNode *node = allocateNode();
    node->entry = entry;

    if (list->head == NULL) {
        list->head = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
    }

    list->tail = node;
    list->size++;
}

void CTLLinkedListAddFirst(CTLLinkedList list, const void *entry) {
    struct __CTLLinkedListNode *node = allocateNode();
    node->entry = entry;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        struct __CTLLinkedListNode *prevHead = list->head;
        node->next = prevHead;
        list->head = node;
    }
    list->size++;
}


CTLLinkedList CTLLinkedListFilter(CTLLinkedList list, CTLpredicate filter) {
    struct __CTLLinkedListNode *head = list->head;
    while (head != NULL) {
        if (!filter(head->entry)) {
            head->prev->next = head->next;
            free(head);
        }
    }
    return list;
}

void CTLLinkedListClear(CTLLinkedList list) {
    struct __CTLLinkedListNode *node = list->head;
    while (node != NULL) {
        struct __CTLLinkedListNode *next = node->next;
        free(node);
        node = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = EMPTY;
}

const void *CTLLinkedListGetFirst(CTLLinkedList list) {
    if (CTLLinkedListIsEmpty(list)) {
        return NULL;
    } else {
        return list->head->entry;
    }
}

const void *CTLLinkedListGetLast(CTLLinkedList list) {
    if (CTLLinkedListIsEmpty(list)) {
        return NULL;
    } else {
        return list->tail->entry;
    }
}

const void *CTLLinkedListFind(CTLLinkedList list, CTLpredicate predicate) {
    if (list->size == EMPTY) {
        return NULL;
    }
    struct __CTLLinkedListNode *head = list->head;
    while (head != NULL) {
        if (predicate(head->entry)) {
            return head->entry;
        }
        head = head->next;
    }
    return NULL;
}

CTLLinkedList CTLLinkedListReverse(CTLLinkedList list) {
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

void CTLLinkedListRemove(CTLLinkedList list, const void *entry, CTLCompareFunction compareFunction) {
    struct __CTLLinkedListNode *node = list->head;
    while (node != NULL) {
        if (compareFunction(entry, node->entry)) {
            node->prev->next = node->next;
            free(node);
        }
        node = node->next;
    }
}

int64_t CTLLinkedListIndexOf(CTLLinkedList list, const void *entry, CTLCompareFunction compareFuntion) {
    struct __CTLLinkedListNode *node = list->head;
    size_t index = 0;
    while (node != NULL) {
        if (compareFuntion(entry, node->entry)) {
            return index;
        }
        node = node->next;
        index++;
    }
    return -1;
} 

bool CTLLinkedListContains(CTLLinkedList list, const void *entry, CTLCompareFunction compareFuntion) {
    struct __CTLLinkedListNode *node = list->head;
    while (node != NULL) {
        if (compareFuntion(entry, node->entry)) {
            return true;
        }
        node = node->next;
    }
    return false;
}