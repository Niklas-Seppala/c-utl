#include "test.h"

#include "ctl/io/logger.h"
#include "testhashmap.h"
#include "testlinkedlist.h"
#include "testvector.h"

#define SUCCESS_DIVIDER TERM_GRN "█" TERM_RSET
#define FAILURE_DIVIDER TERM_RED "█" TERM_RSET

Suite *test_add_case(Suite *suite, const TTest *tc) {
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, tc);
    suite_add_tcase(suite, tc_core);
    return suite;
}

void asd(const char *name, int fail, int total) {
    int failPercent = (int)round(fail / (float)total * 100);
    int successPercent = 100 - failPercent;

    int len = strlen(name);
    printf("--------------------------------------------------\n");
    for (int i = 0; i < 50 / 2 - (len / 2); i++) {
        printf(" ");
    }
    printf("%s\n", name);
    printf("--------------------------------------------------\n");

    if (failPercent > 0) {
        printf("%s %d ", FAILURE_DIVIDER FAILURE_DIVIDER, fail);
    }
    if (successPercent > 0) {
        printf("%s %d ", SUCCESS_DIVIDER SUCCESS_DIVIDER, total - fail);
    }
    printf("\n");

    if (failPercent > 0) {
        for (int i = 0; i < failPercent / 4; i++) {
            printf("%s", " ");
        }
        printf("%d%%", failPercent);
    }
    if (successPercent > 0) {
        for (int i = 0; i < successPercent / 4 + failPercent / 4 - 3; i++) {
            printf("%s", " ");
        }
        printf("%d%%", successPercent);
    }
    printf("\n");

    for (int i = 0; i < failPercent / 2; i++) {
        printf("%s", FAILURE_DIVIDER);
    }
    for (int i = 0; i < successPercent / 2; i++) {
        printf("%s", SUCCESS_DIVIDER);
    }
    printf("\n");
    printf("--------------------------------------------------\n");
}

int main(void) {
    CTLLogSetup();
    int fail_count = 0;
    int total = 0;

    const char *name = "LinkedList";
    Suite *suite = suite_create(name);
    test_linked_list_load_cases(suite);
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_VERBOSE);
    fail_count = srunner_ntests_failed(runner);
    total = srunner_ntests_run(runner);
    srunner_free(runner);

    const char *name2 = "HashMap";
    Suite *suite2 = suite_create(name2);
    loadHashMapTestCases(suite2);
    SRunner *runner2 = srunner_create(suite2);
    srunner_run_all(runner2, CK_VERBOSE);
    int fail_count2 = srunner_ntests_failed(runner2);
    int total2 = srunner_ntests_run(runner2);
    srunner_free(runner2);

    const char *name3 = "Vector";
    Suite *suite3 = suite_create(name3);
    loadVectorTestCases(suite3);
    SRunner *runner3 = srunner_create(suite3);
    srunner_run_all(runner3, CK_VERBOSE);
    int fail_count3 = srunner_ntests_failed(runner3);
    int total3 = srunner_ntests_run(runner3);
    srunner_free(runner3);

    asd(name, fail_count, total);
    asd(name2, fail_count2, total2);
    asd(name3, fail_count3, total3);
    asd("All Tests", fail_count2 + fail_count + fail_count3,
        total + total2 + total3);

    CTLLogTeardown();
    return (fail_count + fail_count2 + fail_count3 == 0) ? EXIT_SUCCESS
                                                         : EXIT_FAILURE;
}
