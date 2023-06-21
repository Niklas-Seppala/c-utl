#include "testvector.h"

#include "ctl/linkedlist.h"
#include "ctl/vector.h"
#include "test.h"

#define START_CAP 10
#define VALUES_LEN 20
int intValues[VALUES_LEN] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
                             10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

inline static bool isGreaterOrEqualToTen(const void *wrapperInteger) {
    int value = *(int *)wrapperInteger;
    return value >= 10;
}

inline static CTLVector getIntegerVector(void) {
    return CTLVectorHeapAlloc(CTLCompareInt, START_CAP);
}

START_TEST(testVectorCreate) {
    CTLVector vec = getIntegerVector();
    ck_assert_ptr_nonnull(vec);
    ck_assert_int_eq(0, CTLVectorSize(vec));
    ck_assert_int_eq(START_CAP, CTLVectorCapacity(vec));
    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorFree) {
    CTLVector vec = getIntegerVector();
    CTLVectorFree(&vec);
    ck_assert_ptr_null(vec);
}
END_TEST

START_TEST(testVectorIsEmpty) {
    CTLVector vec = getIntegerVector();
    ck_assert(CTLVectorIsEmpty(vec));
    CTLVectorAdd(vec, intValues);
    ck_assert(!CTLVectorIsEmpty(vec));
    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorAdd) {
    CTLVector vec = getIntegerVector();
    ck_assert_int_eq(0, CTLVectorSize(vec));

    for (size_t i = 0; i < VALUES_LEN; i++) {
        int *addr = intValues + i;
        CTLVectorAdd(vec, addr);
        ck_assert_int_eq(i + 1, CTLVectorSize(vec));
        ck_assert_ptr_eq(addr, CTLVectorGet(vec, i));
    }

    size_t newCap = CTLVectorCapacity(vec);
    ck_assert_int_eq(START_CAP << 1, newCap);

    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorContains) {
    CTLVector vec = getIntegerVector();
    ck_assert_int_eq(0, CTLVectorSize(vec));

    CTLVectorAdd(vec, intValues + 0);
    CTLVectorAdd(vec, intValues + 1);
    ck_assert_int_eq(2, CTLVectorSize(vec));

    ck_assert(CTLVectorContains(vec, intValues));
    ck_assert(CTLVectorContains(vec, intValues + 1));
    ck_assert(!CTLVectorContains(vec, intValues + 2));

    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorContainsAll) {
    CTLVector vec = getIntegerVector();
    ck_assert_int_eq(0, CTLVectorSize(vec));

    CTLLinkedList list = CTLLinkedListCreate();
    CTLLinkedListAdd(list, intValues + 0);
    CTLLinkedListAdd(list, intValues + 1);
    CTLIterator values = CTLLinkedListIterator(list);

    ck_assert(!CTLVectorContainsAll(vec, values));
    CTLIteratorResetHead(values);

    CTLVectorAddAll(vec, values);
    CTLIteratorResetHead(values);

    ck_assert(CTLVectorContainsAll(vec, values));

    CTLLinkedListFree(&list);
    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorInsert) {
    CTLVector vec = getIntegerVector();
    ck_assert_int_eq(0, CTLVectorSize(vec));

    int value = 1000;
    CTLVectorInsert(vec, 0, &value);
    ck_assert_int_eq(1, CTLVectorSize(vec));

    for (size_t i = 0; i < VALUES_LEN; i++) {
        int *addr = intValues + i;
        CTLVectorAdd(vec, addr);
        ck_assert_int_eq(i + 2, CTLVectorSize(vec));
        ck_assert_ptr_eq(addr, CTLVectorGet(vec, i + 1));
    }

    size_t newCap = CTLVectorCapacity(vec);
    ck_assert_int_eq(START_CAP << 2, newCap);

    size_t lastIndex = CTLVectorSize(vec) - 1;
    const void *valueAtLastIndex = CTLVectorGet(vec, lastIndex);
    const void *valueAtIndex_10 = CTLVectorGet(vec, 10);

    CTLVectorInsert(vec, 10, &value);
    ck_assert_int_eq(VALUES_LEN + 2, CTLVectorSize(vec));

    ck_assert_ptr_eq(&value, CTLVectorGet(vec, 10));
    ck_assert_ptr_eq(valueAtIndex_10, CTLVectorGet(vec, 11));
    ck_assert_ptr_eq(valueAtLastIndex, CTLVectorGet(vec, lastIndex + 1));

    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorRemoveAt) {
    CTLVector vec = getIntegerVector();

    for (size_t i = 0; i < VALUES_LEN; i++) {
        int *addr = intValues + i;
        CTLVectorAdd(vec, addr);
        ck_assert_int_eq(i + 1, CTLVectorSize(vec));
        ck_assert_ptr_eq(addr, CTLVectorGet(vec, i));
    }

    ck_assert_int_eq(1, *(int *)CTLVectorRemoveAt(vec, 1));
    ck_assert_int_eq(2, *(int *)CTLVectorRemoveAt(vec, 1));
    ck_assert_int_eq(3, *(int *)CTLVectorRemoveAt(vec, 1));
    ck_assert_int_eq(10, *(int *)CTLVectorRemoveAt(vec, 7));

    ck_assert_int_eq(16, CTLVectorSize(vec));

    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorRemoveRange) {
    CTLVector vec = getIntegerVector();

    // Does nothing, empty vector.
    CTLVectorRemoveRange(vec, 0, 0);

    for (size_t i = 0; i < VALUES_LEN; i++) {
        CTLVectorAdd(vec, intValues + i);
    }

    size_t size = CTLVectorSize(vec);
    const void *second = CTLVectorGet(vec, 1);
    CTLVectorRemoveRange(vec, 0, 1);
    ck_assert_int_eq(size - 1, CTLVectorSize(vec));
    const void *first = CTLVectorGet(vec, 0);
    // Second (i=1) now should be i=0
    ck_assert_ptr_eq(second, first);

    // Clear the rest of the vector.
    CTLVectorRemoveRange(vec, 0, CTLVectorSize(vec));
    ck_assert_int_eq(0, CTLVectorSize(vec));

    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorRemoveIf) {
    CTLVector vec = getIntegerVector();

    for (size_t i = 0; i < VALUES_LEN; i++) {
        int *addr = intValues + i;
        CTLVectorAdd(vec, addr);
    }
    ck_assert_int_eq(20, CTLVectorSize(vec));
    CTLVectorRemoveIf(vec, isGreaterOrEqualToTen);

    ck_assert_int_eq(10, CTLVectorSize(vec));
    ck_assert_ptr_eq(intValues, CTLVectorGet(vec, 0));
    ck_assert_ptr_eq(intValues + 9, CTLVectorGet(vec, 9));

    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorRetainIf) {
    CTLVector vec = getIntegerVector();

    for (size_t i = 0; i < VALUES_LEN; i++) {
        int *addr = intValues + i;
        CTLVectorAdd(vec, addr);
    }
    ck_assert_int_eq(20, CTLVectorSize(vec));
    CTLVectorRetainIf(vec, isGreaterOrEqualToTen);
    ck_assert_int_eq(10, CTLVectorSize(vec));

    ck_assert_ptr_eq(intValues + 10, CTLVectorGet(vec, 0));
    ck_assert_ptr_eq(intValues + 19, CTLVectorGet(vec, 9));

    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorClear) {
    CTLVector vec = getIntegerVector();

    for (size_t i = 0; i < VALUES_LEN; i++) {
        int *addr = intValues + i;
        CTLVectorAdd(vec, addr);
    }
    ck_assert_int_eq(20, CTLVectorSize(vec));
    CTLVectorClear(vec);
    ck_assert_int_eq(0, CTLVectorSize(vec));

    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorIterator) {
    CTLVector vec = getIntegerVector();
    for (size_t i = 0; i < VALUES_LEN; i++) {
        int *addr = intValues + i;
        CTLVectorAdd(vec, addr);
    }
    ck_assert_int_eq(20, CTLVectorSize(vec));

    CTLIterator iter = CTLVectorIterator(vec);
    size_t index = 0;
    while (CTLIteratorHasNext(iter)) {
        ck_assert_int_eq(index, CTLIteratorGetIndex(iter));
        const int value = *(int *)CTLIteratorNext(iter);
        ck_assert_int_eq(intValues[index], value);
        index++;
    }

    CTLIteratorFree(&iter);
    CTLVectorFree(&vec);
}
END_TEST

int forEachIndex = 0;  // :)
static void consume(const void *entry) {
    ck_assert_ptr_eq(intValues + forEachIndex, entry);
    forEachIndex++;
}
START_TEST(testVectorForEach) {
    CTLVector vec = getIntegerVector();
    for (size_t i = 0; i < VALUES_LEN; i++) {
        int *addr = intValues + i;
        CTLVectorAdd(vec, addr);
    }
    ck_assert_int_eq(20, CTLVectorSize(vec));

    CTLVectorForEach(vec, consume);
    forEachIndex = 0;
    CTLVectorFree(&vec);
}
END_TEST

static void consumeVarArgs(const void *entry, va_list args) {
    int *index = va_arg(args, int *);
    ck_assert_ptr_eq(intValues + *index, entry);
    *index = *index + 1;
}
START_TEST(testVectorForEachVarArgs) {
    CTLVector vec = getIntegerVector();
    for (size_t i = 0; i < VALUES_LEN; i++) {
        int *addr = intValues + i;
        CTLVectorAdd(vec, addr);
    }
    ck_assert_int_eq(20, CTLVectorSize(vec));

    int i = 0;
    CTLVectorForEachVarArg(vec, consumeVarArgs, &i);

    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorIteratorReset) {
    CTLVector vec = getIntegerVector();

    for (size_t i = 0; i < VALUES_LEN; i++) {
        int *addr = intValues + i;
        CTLVectorAdd(vec, addr);
    }
    ck_assert_int_eq(20, CTLVectorSize(vec));

    CTLIterator iter = CTLVectorIterator(vec);

    size_t index = 0;
    while (CTLIteratorHasNext(iter)) {
        ck_assert_int_eq(index, CTLIteratorGetIndex(iter));
        const int value = *(int *)CTLIteratorNext(iter);
        ck_assert_int_eq(intValues[index], value);
        index++;
    }
    CTLIteratorResetHead(iter);

    index = 0;
    while (CTLIteratorHasNext(iter)) {
        ck_assert_int_eq(index, CTLIteratorGetIndex(iter));
        const int value = *(int *)CTLIteratorNext(iter);
        ck_assert_int_eq(intValues[index], value);
        index++;
    }

    CTLIteratorFree(&iter);
    CTLVectorFree(&vec);
}
END_TEST

START_TEST(testVectorRemove) {
    CTLVector vec = getIntegerVector();

    ck_assert_int_eq(0, CTLVectorSize(vec));
    ck_assert(!CTLVectorRemove(vec, intValues + 0));
    ck_assert_int_eq(0, CTLVectorSize(vec));

    CTLVectorAdd(vec, intValues + 0);
    ck_assert_int_eq(1, CTLVectorSize(vec));

    ck_assert(CTLVectorRemove(vec, intValues + 0));
    ck_assert_int_eq(0, CTLVectorSize(vec));

    CTLVectorFree(&vec);
}
END_TEST

void loadVectorTestCases(Suite *suite) {
    test_add_case(suite, testVectorCreate);
    test_add_case(suite, testVectorFree);
    test_add_case(suite, testVectorIsEmpty);
    test_add_case(suite, testVectorAdd);
    test_add_case(suite, testVectorInsert);
    test_add_case(suite, testVectorContains);
    test_add_case(suite, testVectorContainsAll);
    test_add_case(suite, testVectorRemove);
    test_add_case(suite, testVectorRemoveAt);
    test_add_case(suite, testVectorRemoveRange);
    test_add_case(suite, testVectorRemoveIf);
    test_add_case(suite, testVectorRetainIf);
    test_add_case(suite, testVectorClear);
    test_add_case(suite, testVectorIterator);
    test_add_case(suite, testVectorIteratorReset);
    test_add_case(suite, testVectorForEach);
    test_add_case(suite, testVectorForEachVarArgs);
}