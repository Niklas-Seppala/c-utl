#include "test.h"
#include "testlinkedlist.h"

#include "../include/ctl/io/logger.h"

#define SUCCESS_DIVIDER TERM_GRN "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" TERM_RSET
#define FAILURE_DIVIDER TERM_RED "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n" TERM_RSET

Suite *test_add_case(Suite *suite, const TTest *tc)
{
    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, tc);
    suite_add_tcase(suite, tc_core);
    return suite;
}

int main(void)
{
    CTLLogSetup();
    int fail_count = 0;
    int total_count = 0;

    Suite *suite = suite_create("CTL");

    test_linked_list_load_cases(suite);

    SRunner *runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);
    fail_count = srunner_ntests_failed(runner);
    total_count = srunner_ntests_run(runner);
    srunner_free(runner);

    const int success = fail_count == 0;
    const char *DIVIDER = success ? SUCCESS_DIVIDER : FAILURE_DIVIDER;

    printf("\n%s", DIVIDER);
    printf(" %.1f%%    TOTAL RUN: %d    PASSED: %d    FAILED: %d\n",
           (double)total_count / (double)(total_count - fail_count) * 100.0,
           total_count, 
           total_count - fail_count, fail_count);
    
    printf("%s", DIVIDER);

    CTLLogTeardown();
    return (fail_count == 0) ? EXIT_SUCCESS : EXIT_FAILURE;  
}
