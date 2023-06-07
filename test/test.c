#include "test.h"
#include "testlinkedlist.h"

#include "../include/ctl/io/logger.h"

#define SUCCESS_DIVIDER TERM_GRN "█" TERM_RSET
#define FAILURE_DIVIDER TERM_RED "█" TERM_RSET

Suite *test_add_case(Suite *suite, const TTest *tc)
{
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, tc);
    suite_add_tcase(suite, tc_core);
    return suite;
}

void asd(int fail, int total) {
    int failPercent = (int) round(fail / (float)total * 100);
    int successPercent = 100 - failPercent;

    printf("\n");

    if (failPercent > 0) {
        printf("%s %d ", FAILURE_DIVIDER FAILURE_DIVIDER, fail);
    }
    if (successPercent > 0) {
        printf("%s %d\n\n", SUCCESS_DIVIDER SUCCESS_DIVIDER, total-fail);
    }
    
    if (failPercent > 0) {
        for (int i = 0; i < failPercent / 4; i++) {
            printf("%s", " ");
        }
        printf("%d%%", failPercent);
    } 
    if (successPercent > 0) {
        for (int i = 0; i < successPercent / 4 + failPercent / 4-3; i++) {
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
    printf("\n\n");
}

int main(void)
{
    CTLLogSetup();
    int fail_count = 0;
    int total = 0;

    Suite *suite = suite_create("CTL");

    test_linked_list_load_cases(suite);

    SRunner *runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    fail_count = srunner_ntests_failed(runner);
    total = srunner_ntests_run(runner);
    srunner_free(runner);

    

    const int success = fail_count == 0;
    const char *DIVIDER = success ? SUCCESS_DIVIDER : FAILURE_DIVIDER;
    asd(fail_count, total);

    CTLLogTeardown();
    return (fail_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;  
}
