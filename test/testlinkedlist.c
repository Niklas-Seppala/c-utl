#include "testlinkedlist.h"
#include "ctl/linkedlist.h"
#include "test.h"

static const char *item1 = "item1";
static const char *item2 = "item2";
static const char *item3 = "item3";
static const char *item4 = "item4";

typedef struct house {
    int rooms;
    char *name;
} House;

bool houseByName(const void *item) {
    if (item == NULL) return false;
    const House *aHouse = (House *)item;
    return CTLCompareString(aHouse->name, "House 2") == EQ;
}

static bool strLastCharIs2(const void *item) {
    const char *str = item;
    return str[strlen(str) - 1] == '2';
}

START_TEST(test_linked_list_alloc) {
    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_ptr_ne(NULL, list);
    ck_assert_int_eq(0, CTLLinkedListSize(list));
    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(test_linked_list_free) {
    CTLLinkedList list = CTLLinkedListCreate();
    CTLLinkedListFree(&list);
    ck_assert_ptr_eq(NULL, list);
}
END_TEST

START_TEST(testLinkedListAdd) {
    CTLLinkedList list = CTLLinkedListCreate();
    const char *tail = "tail";

    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListAdd(list, NULL);
    ck_assert_int_eq(1, CTLLinkedListSize(list));
    ck_assert_ptr_eq(NULL, CTLLinkedListGetLast(list));

    CTLLinkedListAdd(list, NULL);
    ck_assert_int_eq(2, CTLLinkedListSize(list));
    ck_assert_ptr_eq(NULL, CTLLinkedListGetLast(list));

    CTLLinkedListAdd(list, tail);
    ck_assert_int_eq(3, CTLLinkedListSize(list));
    ck_assert_ptr_eq(tail, CTLLinkedListGetLast(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListAddAll) {
    const char *head = "head";
    const char *tail = "tail";

    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListAddAll(list, 4, head, item1, NULL, tail);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    ck_assert_ptr_eq(head, CTLLinkedListGetFirst(list));
    ck_assert_ptr_eq(tail, CTLLinkedListGetLast(list));
    ck_assert_ptr_eq(item1, CTLLinkedListGet(list, 1));
    ck_assert_ptr_eq(NULL, CTLLinkedListGet(list, 2));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListSize) {
    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListAdd(list, NULL);
    ck_assert_int_eq(1, CTLLinkedListSize(list));
    CTLLinkedListAdd(list, NULL);
    ck_assert_int_eq(2, CTLLinkedListSize(list));

    CTLLinkedListRemoveAt(list, 0);
    ck_assert_int_eq(1, CTLLinkedListSize(list));
    CTLLinkedListRemoveAt(list, 0);
    ck_assert_int_eq(0, CTLLinkedListSize(list));
    CTLLinkedListRemoveAt(list, 0);
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListIsEmpty) {
    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_int_eq(true, CTLLinkedListIsEmpty(list));
    CTLLinkedListAdd(list, NULL);
    ck_assert_int_eq(false, CTLLinkedListIsEmpty(list));

    CTLLinkedListRemoveAt(list, 0);
    ck_assert_int_eq(true, CTLLinkedListIsEmpty(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListFind) {
    CTLLinkedList list = CTLLinkedListCreate();
    const char *notFound = "notFound";

    CTLLinkedListAddAll(list, 3, item1, item2, item3);

    char *findResult = NULL;
    // Should be found.
    findResult = (void *)CTLLinkedListFind(list, item1, CTLCompareString);
    ck_assert_ptr_ne(NULL, findResult);

    findResult = (void *)CTLLinkedListFind(list, item2, CTLCompareString);
    ck_assert_ptr_ne(NULL, findResult);

    findResult = (void *)CTLLinkedListFind(list, item3, CTLCompareString);
    ck_assert_ptr_ne(NULL, findResult);

    // Should not be found.
    findResult = (void *)CTLLinkedListFind(list, notFound, CTLCompareString);
    ck_assert_ptr_eq(NULL, findResult);

    findResult = (void *)CTLLinkedListFind(list, NULL, CTLCompareString);
    ck_assert_ptr_eq(NULL, findResult);

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListInsertAt) {
    CTLLinkedList list = CTLLinkedListCreate();
    size_t insertedAt;

    insertedAt = CTLLinkedListInsertAt(list, item1, 5);
    ck_assert_int_eq(0, insertedAt);
    ck_assert_int_eq(1, CTLLinkedListSize(list));

    insertedAt = CTLLinkedListInsertAt(list, item1, 5);
    ck_assert_int_eq(1, insertedAt);
    ck_assert_int_eq(2, CTLLinkedListSize(list));

    const char *first = "first";
    insertedAt = CTLLinkedListInsertAt(list, first, 0);
    ck_assert_int_eq(0, insertedAt);
    ck_assert_ptr_eq(first, CTLLinkedListGetFirst(list));
    ck_assert_int_eq(3, CTLLinkedListSize(list));

    insertedAt = CTLLinkedListInsertAt(list, item1, 1);
    ck_assert_int_eq(1, insertedAt);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListGet) {
    char *item = NULL;
    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    item = (void *)CTLLinkedListGet(list, 0);
    ck_assert_ptr_null(item);

    CTLLinkedListAddAll(list, 4, item1, item2, item3, item4);

    ck_assert_int_eq(4, CTLLinkedListSize(list));

    item = (void *)CTLLinkedListGet(list, 0);
    ck_assert_ptr_eq(item1, item);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    item = (void *)CTLLinkedListGet(list, 1);
    ck_assert_ptr_eq(item2, item);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    item = (void *)CTLLinkedListGet(list, 2);
    ck_assert_ptr_eq(item3, item);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    item = (void *)CTLLinkedListGet(list, 3);
    ck_assert_ptr_eq(item4, item);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    item = (void *)CTLLinkedListGet(list, 526);
    ck_assert_ptr_null(item);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    CTLLinkedListFree(&list);
}
END_TEST


START_TEST(testLinkedListRemoveAt) {
    CTLLinkedList list = CTLLinkedListCreate();
    char *removedItem = NULL;

    CTLLinkedListAddAll(list, 4, item1, item2, item3, item4);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    removedItem = (void *)CTLLinkedListRemoveAt(list, 0);
    ck_assert_ptr_eq(item1, removedItem);
    ck_assert_int_eq(3, CTLLinkedListSize(list));

    removedItem = (void *)CTLLinkedListRemoveAt(list, 10);
    ck_assert_ptr_eq(NULL, removedItem);
    ck_assert_int_eq(3, CTLLinkedListSize(list));

    removedItem = (void *)CTLLinkedListRemoveAt(list, 1);
    ck_assert_ptr_eq(item3, removedItem);
    ck_assert_int_eq(2, CTLLinkedListSize(list));

    removedItem = (void *)CTLLinkedListRemoveAt(list, 1);
    ck_assert_ptr_eq(item4, removedItem);
    ck_assert_int_eq(1, CTLLinkedListSize(list));

    removedItem = (void *)CTLLinkedListRemoveAt(list, 0);
    ck_assert_ptr_eq(item2, removedItem);
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    removedItem = (void *)CTLLinkedListRemoveAt(list, 0);
    ck_assert_ptr_eq(NULL, removedItem);
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListAddFirst) {
    CTLLinkedList list = CTLLinkedListCreate();

    CTLLinkedListAddFirst(list, item1);
    ck_assert_int_eq(1, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item1, CTLLinkedListGetFirst(list));

    CTLLinkedListAddFirst(list, item2);
    ck_assert_int_eq(2, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item2, CTLLinkedListGetFirst(list));

    CTLLinkedListAddFirst(list, item3);
    ck_assert_int_eq(3, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item3, CTLLinkedListGetFirst(list));

    CTLLinkedListAddFirst(list, item4);
    ck_assert_int_eq(4, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item4, CTLLinkedListGetFirst(list));

    CTLLinkedListAddFirst(list, NULL);
    ck_assert_int_eq(5, CTLLinkedListSize(list));
    ck_assert_ptr_eq(NULL, CTLLinkedListGetFirst(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListClear) {
    CTLLinkedList list = CTLLinkedListCreate();

    CTLLinkedListAddFirst(list, item1);
    ck_assert_int_eq(1, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item1, CTLLinkedListGetFirst(list));

    CTLLinkedListAddFirst(list, item2);
    ck_assert_int_eq(2, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item2, CTLLinkedListGetFirst(list));

    CTLLinkedListClear(list);
    ck_assert_int_eq(0, CTLLinkedListSize(list));
    ck_assert_ptr_eq(NULL, CTLLinkedListGetFirst(list));
    ck_assert_ptr_eq(NULL, CTLLinkedListGetLast(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListGetFirst) {
    CTLLinkedList list = CTLLinkedListCreate();

    ck_assert_int_eq(0, CTLLinkedListSize(list));
    ck_assert_ptr_eq(NULL, CTLLinkedListGetFirst(list));

    CTLLinkedListAdd(list, item1);
    ck_assert_int_eq(1, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item1, CTLLinkedListGetFirst(list));

    CTLLinkedListAddFirst(list, item2);
    ck_assert_int_eq(2, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item2, CTLLinkedListGetFirst(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListGetLast) {
    CTLLinkedList list = CTLLinkedListCreate();

    ck_assert_int_eq(0, CTLLinkedListSize(list));
    ck_assert_ptr_eq(NULL, CTLLinkedListGetLast(list));

    CTLLinkedListAdd(list, item1);
    ck_assert_int_eq(1, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item1, CTLLinkedListGetLast(list));

    CTLLinkedListAdd(list, item2);
    ck_assert_int_eq(2, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item2, CTLLinkedListGetLast(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListRemove) {
    CTLLinkedList list = CTLLinkedListCreate();
    char *removed = NULL;

    ck_assert_int_eq(0, CTLLinkedListSize(list));

    removed = (void *)CTLLinkedListRemove(list, item1, CTLCompareString);
    ck_assert_ptr_null(removed);
    
    CTLLinkedListAddAll(list, 2, item1, item2);
    ck_assert_int_eq(2, CTLLinkedListSize(list));

    removed = (void *)CTLLinkedListRemove(list, item1, CTLCompareString);
    ck_assert_ptr_eq(item1, removed);
    ck_assert_int_eq(1, CTLLinkedListSize(list));

    removed = (void *)CTLLinkedListRemove(list, item2, CTLCompareString);
    ck_assert_ptr_eq(item2, removed);
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListFilter) {
    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListAddAll(list, 4, item1, item2, item3, item4);

    CTLLinkedListFilter(list, strLastCharIs2);
    ck_assert_int_eq(1, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item2, CTLLinkedListGetFirst(list));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListReverse) {
    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListAddAll(list, 3, item1, item2, item3);

    ck_assert_int_eq(3, CTLLinkedListSize(list));
    ck_assert_ptr_eq(item1, CTLLinkedListGetFirst(list));
    ck_assert_ptr_eq(item3, CTLLinkedListGetLast(list));

    CTLLinkedListReverse(list);

    ck_assert_ptr_eq(item3, CTLLinkedListGetFirst(list));
    ck_assert_ptr_eq(item1, CTLLinkedListGetLast(list));
    ck_assert_int_eq(3, CTLLinkedListSize(list));

    ck_assert_ptr_eq(item2, CTLLinkedListGet(list, 1));

    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListContains) {
    const char *missingEntry = "missing";

    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListAddAll(list, 2, item1, item2);
    ck_assert_int_eq(2, CTLLinkedListSize(list));

    ck_assert(CTLLinkedListContains(list, item2, CTLCompareString));
    ck_assert(CTLLinkedListContains(list, item1, CTLCompareString));
    ck_assert(!CTLLinkedListContains(list, missingEntry, CTLCompareString));

    CTLLinkedListFree(&list);
}
END_TEST


START_TEST(testLinkedListIndexOf) {
    const char *missingEntry = "missing";
    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListAddAll(list, 4, item1, item2, item3, item4);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    ck_assert_int_eq(0, CTLLinkedListIndexOf(list, item1, CTLCompareString));
    ck_assert_int_eq(1, CTLLinkedListIndexOf(list, item2, CTLCompareString));
    ck_assert_int_eq(2, CTLLinkedListIndexOf(list, item3, CTLCompareString));
    ck_assert_int_eq(3, CTLLinkedListIndexOf(list, item4, CTLCompareString));

    ck_assert_int_eq(-1, CTLLinkedListIndexOf(list, missingEntry, CTLCompareString));
    
    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListCopy) {
    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_int_eq(0, CTLLinkedListSize(list));

    CTLLinkedListAddAll(list, 4, item1, item2, item3, item4);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    ck_assert_ptr_eq(item1, CTLLinkedListGet(list, 0));
    ck_assert_ptr_eq(item2, CTLLinkedListGet(list, 1));
    ck_assert_ptr_eq(item3, CTLLinkedListGet(list, 2));
    ck_assert_ptr_eq(item4, CTLLinkedListGet(list, 3));

    CTLLinkedList copy = CTLLinkedListCopy(list);

    ck_assert_int_eq(4, CTLLinkedListSize(copy));
    ck_assert_ptr_eq(item1, CTLLinkedListGet(copy, 0));
    ck_assert_ptr_eq(item2, CTLLinkedListGet(copy, 1));
    ck_assert_ptr_eq(item3, CTLLinkedListGet(copy, 2));
    ck_assert_ptr_eq(item4, CTLLinkedListGet(copy, 3));

    CTLLinkedListFree(&copy);
    CTLLinkedListFree(&list);
}
END_TEST

START_TEST(testLinkedListIterator) {
    CTLLinkedList list = CTLLinkedListCreate();
    CTLLinkedListAddAll(list, 4, item1, item2, item3, item4);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    CTLIterator iter = CTLLinkedListIterator(list);

    ck_assert(CTLIteratorHasNext(iter));
    ck_assert_ptr_eq(item1, CTLIteratorNext(iter));
    ck_assert(CTLIteratorHasNext(iter));
    ck_assert_ptr_eq(item2, CTLIteratorNext(iter));
    ck_assert(CTLIteratorHasNext(iter));
    ck_assert_ptr_eq(item3, CTLIteratorNext(iter));
    ck_assert(CTLIteratorHasNext(iter));
    ck_assert_ptr_eq(item4, CTLIteratorNext(iter));
    ck_assert(!CTLIteratorHasNext(iter));

    CTLLinkedListIteratorReset(list);

    ck_assert(CTLIteratorHasNext(iter));
    ck_assert_ptr_eq(item1, CTLIteratorNext(iter));
    ck_assert(CTLIteratorHasNext(iter));
    ck_assert_ptr_eq(item2, CTLIteratorNext(iter));
    ck_assert(CTLIteratorHasNext(iter));
    ck_assert_ptr_eq(item3, CTLIteratorNext(iter));
    ck_assert(CTLIteratorHasNext(iter));
    ck_assert_ptr_eq(item4, CTLIteratorNext(iter));
    ck_assert(!CTLIteratorHasNext(iter));

    CTLLinkedListFree(&list);
}
END_TEST

static void checkEntry(const void *entry) {
    ck_assert_ptr_nonnull(entry);
    printf("%s\n", (char *)entry);
}

START_TEST(testLinkedListForEach) {
    CTLLinkedList list = CTLLinkedListCreate();
    CTLLinkedListAddAll(list, 4, item1, item2, item3, item4);
    ck_assert_int_eq(4, CTLLinkedListSize(list));

    CTLLinkedListForEach(list, checkEntry);
    CTLLinkedListFree(&list);
}
END_TEST


void test_linked_list_load_cases(Suite *suite) {
    test_add_case(suite, test_linked_list_alloc);
    test_add_case(suite, test_linked_list_free);
    test_add_case(suite, testLinkedListSize);
    test_add_case(suite, testLinkedListIsEmpty);
    test_add_case(suite, testLinkedListFind);
    test_add_case(suite, testLinkedListAdd);
    test_add_case(suite, testLinkedListAddAll);
    test_add_case(suite, testLinkedListInsertAt);
    test_add_case(suite, testLinkedListGet);
    test_add_case(suite, testLinkedListRemoveAt);
    test_add_case(suite, testLinkedListAddFirst);
    test_add_case(suite, testLinkedListClear);
    test_add_case(suite, testLinkedListGetFirst);
    test_add_case(suite, testLinkedListGetLast);
    test_add_case(suite, testLinkedListRemove);
    test_add_case(suite, testLinkedListFilter);
    test_add_case(suite, testLinkedListReverse);
    test_add_case(suite, testLinkedListContains);
    test_add_case(suite, testLinkedListIndexOf);
    test_add_case(suite, testLinkedListCopy);
    test_add_case(suite, testLinkedListIterator);
    test_add_case(suite, testLinkedListForEach);
}