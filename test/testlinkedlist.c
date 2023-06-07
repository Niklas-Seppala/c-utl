#include "testlinkedlist.h"
#include "ctl/linkedlist.h"
#include "test.h"

START_TEST(test_linked_list_alloc) {
    CTLLinkedList list = CTLLinkedListCreate();
    ck_assert_ptr_ne(NULL, NULL);
    ck_assert_int_eq(0, CTLLinkedListSize(list));
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

void test_linked_list_load_cases(Suite *suite) {
    test_add_case(suite, test_linked_list_alloc);
    test_add_case(suite, testLinkedListSize);
    test_add_case(suite, testLinkedListIsEmpty);
}