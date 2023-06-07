#if !defined(GUARD_TEST_H)
#define GUARD_TEST_H

#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "../include/ctl/io/logger.h"

typedef void (*TestCase)(int);

#define PRINTF_TEST(format, ...) \
    CTLLogPrintf(CTL_TEST, format, __VA_ARGS__)

Suite *test_add_case(Suite *suite, const TTest *tc);

#endif // GUARD_TEST_H
