#include "testhashmap.h"
#include "ctl/hashmap.h"
#include "test.h"

START_TEST(testHashMapAllocate) {
    CTLHashMap map = CTLHashMapCreate(16, 0.75f, CTLHashSDBM_STRING, CTLCompareString);
    ck_assert_ptr_ne(NULL, map);

    const char *key1 = "key";
    const char *value1 = "value";
    CTLHashMapPut(map, key1, 0, value1);

    CTLHashMapFree(&map);
    ck_assert_ptr_null(map);
}
END_TEST

void loadHashMapTestCases(Suite *suite) {
    test_add_case(suite, testHashMapAllocate);
}