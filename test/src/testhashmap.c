#include "testhashmap.h"

#include "ctl/hashmap.h"
#include "ctl/hashset.h"
#include "test.h"

#define ENTRIES 20

static const char *keys[ENTRIES] = {
    "key-0",  "key-1",  "key-2",  "key-3",  "key-4",  "key-5",  "key-6",
    "key-7",  "key-8",  "key-9",  "key-10", "key-11", "key-12", "key-13",
    "key-14", "key-15", "key-16", "key-17", "key-18", "key-19",
};
static const char *values[ENTRIES] = {
    "value-0",  "value-1",  "value-2",  "value-3",  "value-4",
    "value-5",  "value-6",  "value-7",  "value-8",  "value-9",
    "value-10", "value-11", "value-12", "value-13", "value-14",
    "value-15", "value-16", "value-17", "value-18", "value-19",
};

static CTLHashMap getMap(void) {
    return CTLHashMapCreate(16, 0.75f, CTLHashSDBM_STRING,
                            CTLCompareEntryStringKey,
                            CTLCompareEntryStringValue);
}

START_TEST(testHashMapAllocate) {
    CTLHashMap map = getMap();
    ck_assert_ptr_ne(NULL, map);
    CTLHashMapPut(map, keys[0], 0, values[0]);
    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapFree) {
    CTLHashMap map = getMap();
    CTLHashMapPut(map, keys[0], 0, values[0]);
    CTLHashMapFree(&map);
    ck_assert_ptr_null(map);
}
END_TEST

START_TEST(testHashMapSize) {
    CTLHashMap map = getMap();
    ck_assert_int_eq(0, CTLHashMapSize(map));

    CTLHashMapPut(map, keys[0], 0, values[0]);
    ck_assert_int_eq(1, CTLHashMapSize(map));

    CTLHashMapPut(map, keys[1], 0, values[1]);
    ck_assert_int_eq(2, CTLHashMapSize(map));

    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapIsEmpty) {
    CTLHashMap map = getMap();
    ck_assert(CTLHashMapIsEmpty(map));

    CTLHashMapPut(map, keys[0], 0, values[0]);
    ck_assert_int_eq(1, CTLHashMapSize(map));

    ck_assert(!CTLHashMapIsEmpty(map));

    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapContainsKey) {
    CTLHashMap map = getMap();
    ck_assert(!CTLHashMapContainsKey(map, keys[0], 0));

    CTLHashMapPut(map, keys[0], 0, values[0]);
    ck_assert_int_eq(1, CTLHashMapSize(map));
    ck_assert(CTLHashMapContainsKey(map, keys[0], 0));

    CTLHashMapRemove(map, keys[0], 0);
    ck_assert_int_eq(0, CTLHashMapSize(map));
    ck_assert(!CTLHashMapContainsKey(map, keys[0], 0));

    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapReplace) {
    CTLHashMap map = getMap();

    bool wasReplaced = CTLHashMapReplace(map, keys[0], 0, values[0]);
    ck_assert(!wasReplaced);
    ck_assert_int_eq(1, CTLHashMapSize(map));

    wasReplaced = CTLHashMapReplace(map, keys[0], 0, values[1]);
    ck_assert(wasReplaced);
    ck_assert_int_eq(1, CTLHashMapSize(map));

    const char *value = CTLHashMapGet(map, keys[0], 0);
    ck_assert_ptr_eq(values[1], value);

    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapGet) {
    CTLHashMap map = getMap();
    ck_assert_int_eq(0, CTLHashMapSize(map));

    CTLHashMapPut(map, keys[0], 0, values[0]);
    ck_assert_int_eq(1, CTLHashMapSize(map));

    CTLHashMapPut(map, keys[1], 0, values[1]);
    ck_assert_int_eq(2, CTLHashMapSize(map));

    char *value = (void *)CTLHashMapGet(map, keys[0], 0);
    ck_assert_ptr_eq(values[0], value);

    value = (void *)CTLHashMapGet(map, keys[1], 0);
    ck_assert_ptr_eq(values[1], value);

    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapPut) {
    CTLHashMap map = getMap();
    ck_assert_int_eq(0, CTLHashMapSize(map));

    CTLHashMapPut(map, keys[0], 0, values[0]);
    ck_assert_int_eq(1, CTLHashMapSize(map));

    CTLHashMapPut(map, keys[1], 0, values[1]);
    ck_assert_int_eq(2, CTLHashMapSize(map));

    char *value = (void *)CTLHashMapGet(map, keys[0], 0);
    ck_assert_ptr_eq(values[0], value);

    value = (void *)CTLHashMapGet(map, keys[1], 0);
    ck_assert_ptr_eq(values[1], value);

    CTLHashMapPut(map, keys[0], 0, values[1]);
    ck_assert_int_eq(2, CTLHashMapSize(map));
    value = (void *)CTLHashMapGet(map, keys[0], 0);
    ck_assert_ptr_eq(values[1], value);

    CTLHashMapPut(map, NULL, 0, values[0]);
    value = (void *)CTLHashMapGet(map, NULL, 0);
    ck_assert_ptr_eq(values[0], value);

    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapRemove) {
    CTLHashMap map = getMap();
    ck_assert_int_eq(0, CTLHashMapSize(map));

    CTLHashMapPut(map, keys[0], 0, values[0]);
    ck_assert_int_eq(1, CTLHashMapSize(map));

    CTLHashMapPut(map, keys[1], 0, values[1]);
    ck_assert_int_eq(2, CTLHashMapSize(map));

    char *value = (void *)CTLHashMapRemove(map, keys[0], 0);
    ck_assert_ptr_eq(values[0], value);
    ck_assert_int_eq(1, CTLHashMapSize(map));

    value = (void *)CTLHashMapRemove(map, keys[1], 0);
    ck_assert_ptr_eq(values[1], value);
    ck_assert_int_eq(0, CTLHashMapSize(map));

    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapPutIfAbsent) {
    CTLHashMap map = getMap();
    ck_assert_int_eq(0, CTLHashMapSize(map));

    void *prev = (void *)CTLHashMapPutIfAbsent(map, keys[0], 0, values[0]);
    ck_assert_int_eq(1, CTLHashMapSize(map));
    ck_assert_ptr_eq(values[0], CTLHashMapGet(map, keys[0], 0));
    ck_assert_ptr_null(prev);

    prev = (void *)CTLHashMapPutIfAbsent(map, keys[1], 0, values[1]);
    ck_assert_int_eq(2, CTLHashMapSize(map));
    ck_assert_ptr_eq(values[1], CTLHashMapGet(map, keys[1], 0));
    ck_assert_ptr_null(prev);

    prev = (void *)CTLHashMapPutIfAbsent(map, keys[1], 0, values[0]);
    ck_assert_int_eq(2, CTLHashMapSize(map));
    ck_assert_ptr_eq(values[1], CTLHashMapGet(map, keys[1], 0));
    ck_assert_ptr_eq(values[1], prev);

    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapAllocateKeyList) {
    CTLHashMap map = getMap();
    for (int i = 0; i < ENTRIES; i++) {
        CTLHashMapPut(map, keys[i], 0, values[i]);
    }

    CTLLinkedList keySet = CTLHashMapAllocateKeyList(map);
    ck_assert_int_eq(ENTRIES, CTLLinkedListSize(keySet));
    for (size_t i = 0; i < 2; i++) {
        ck_assert(CTLLinkedListContains(keySet, keys[i], CTLCompareString));
    }

    CTLLinkedListFree(&keySet);
    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapAllocateKeySet) {
    CTLHashMap map = getMap();
    // Store all entries to hashmap.
    for (int i = 0; i < ENTRIES; i++) {
        CTLHashMapPut(map, keys[i], 0, values[i]);
    }

    CTLKeyHashSet keySet = CTLHashMapAllocateKeySet(map);
    ck_assert_int_eq(ENTRIES, CTLHashSetSize(keySet));

    // Check that all keys in map are present in key set.
    for (size_t i = 0; i < ENTRIES; i++) {
        ck_assert(CTLHashSetContains(keySet, keys[i], 0));
    }
    CTLHashSetFree(&keySet);

    CTLHashMapRemove(map, keys[0], 0);

    keySet = CTLHashMapAllocateKeySet(map);
    ck_assert_int_eq(ENTRIES - 1, CTLHashSetSize(keySet));

    // Removed key should not be present anymore.
    ck_assert(!CTLHashSetContains(keySet, keys[0], 0));

    CTLHashSetFree(&keySet);
    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapAllocateValuesList) {
    CTLHashMap map = getMap();
    for (int i = 0; i < ENTRIES; i++) {
        CTLHashMapPut(map, keys[i], 0, values[i]);
    }

    CTLLinkedList valueSet = CTLHashMapAllocateValueList(map);
    ck_assert_int_eq(ENTRIES, CTLLinkedListSize(valueSet));
    for (size_t i = 0; i < 2; i++) {
        ck_assert(CTLLinkedListContains(valueSet, values[i], CTLCompareString));
    }

    CTLLinkedListFree(&valueSet);
    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapClear) {
    CTLHashMap map = getMap();
    ck_assert_int_eq(0, CTLHashMapSize(map));

    CTLHashMapPut(map, keys[0], 0, values[0]);
    ck_assert_int_eq(1, CTLHashMapSize(map));

    CTLHashMapPut(map, values[1], 0, values[1]);
    ck_assert_int_eq(2, CTLHashMapSize(map));

    CTLHashMapClear(map);
    ck_assert_int_eq(0, CTLHashMapSize(map));
    ck_assert_ptr_null(CTLHashMapGet(map, keys[0], 0));
    ck_assert_ptr_null(CTLHashMapGet(map, keys[1], 0));

    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapResizing) {
    CTLHashMap map = getMap();
    ck_assert_int_eq(0, CTLHashMapSize(map));
    for (int i = 0; i < ENTRIES; i++) {
        CTLHashMapPut(map, keys[i], 0, values[i]);
    }
    ck_assert_int_eq(ENTRIES, CTLHashMapSize(map));
    for (int i = 0; i < 20; i++) {
        ck_assert_ptr_eq(values[i], CTLHashMapGet(map, keys[i], 0));
    }
    CTLHashMapFree(&map);
}
END_TEST

static void checkEntry(const void *key, const void *value) {
    ck_assert_ptr_nonnull(key);
    ck_assert_ptr_nonnull(value);
}

START_TEST(testHashMapForEachEntry) {
    CTLHashMap map = getMap();
    for (int i = 0; i < ENTRIES; i++) {
        CTLHashMapPut(map, keys[i], 0, values[i]);
    }
    CTLHashMapForEachEntry(map, checkEntry);

    CTLHashMapFree(&map);
}
END_TEST

START_TEST(testHashMapContainsValue) {
    const char *key1 = "needle-1";
    const char *value1 = "haystack-1";
    const char *key2 = "needle-2";
    const char *value2 = "haystack-2";

    CTLHashMap map = getMap();
    ck_assert(!CTLHashMapContainsValue(map, value1));
    ck_assert(!CTLHashMapContainsValue(map, value2));

    for (int i = 0; i < ENTRIES; i++) {
        CTLHashMapPut(map, keys[i], 0, values[i]);
        ck_assert(!CTLHashMapContainsValue(map, value1));
        ck_assert(!CTLHashMapContainsValue(map, value2));
    }

    CTLHashMapPut(map, key1, 0, value1);
    ck_assert(CTLHashMapContainsValue(map, value1));

    CTLHashMapPut(map, key2, 0, value2);
    ck_assert(CTLHashMapContainsValue(map, value2));

    CTLHashMapFree(&map);
}
END_TEST

void loadHashMapTestCases(Suite *suite) {
    test_add_case(suite, testHashMapAllocate);
    test_add_case(suite, testHashMapFree);
    test_add_case(suite, testHashMapSize);
    test_add_case(suite, testHashMapIsEmpty);
    test_add_case(suite, testHashMapContainsKey);
    test_add_case(suite, testHashMapReplace);
    test_add_case(suite, testHashMapGet);
    test_add_case(suite, testHashMapPut);
    test_add_case(suite, testHashMapRemove);
    test_add_case(suite, testHashMapPutIfAbsent);
    test_add_case(suite, testHashMapAllocateKeyList);
    test_add_case(suite, testHashMapAllocateValuesList);
    test_add_case(suite, testHashMapClear);
    test_add_case(suite, testHashMapResizing);
    test_add_case(suite, testHashMapForEachEntry);
    test_add_case(suite, testHashMapContainsValue);
    test_add_case(suite, testHashMapAllocateKeySet);
}