#include "testlinkedlist.h"
#include "ctl/linkedlist.h"
#include "test.h"

START_TEST(test_linked_list_alloc) {
    CTLLinkedList list = CTLLinkedListCreate();
    CTLLinkedListFree(&list);
}
END_TEST

void test_linked_list_load_cases(Suite *suite) {
    test_add_case(suite, test_linked_list_alloc);
}