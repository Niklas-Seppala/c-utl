#include "ctl/io/logger.h"
#include "ctl/linkedlist.h"
#include "ctl/threads/threadpool.h"
#include "test.h"
#include "testlinkedlist.h"
#include "unistd.h"

static void *printThreadId2(CTLTaskArgs *args) {
    sleep(2);
    return &args->threadInfo.id;
}


static void *printThreadId4(CTLTaskArgs *args) {
    sleep(4);
    return &args->threadInfo.id;
}

START_TEST(testThreadPool) {
    CTLThreadPool pool = CTLThreadPoolCreate("Pool", 8);
    CTLTask *tasks[8];
    for (int i = 0; i < 8; i++) {
        CTLTask *task = calloc(1, sizeof(CTLTask));
        task->args.args = NULL;
        task->task = printThreadId2;
        tasks[i] = task;
        CTLThreadPoolSubmitTask(pool, task);
    }
    CTLThreadPoolShutdownAwait(pool);

    for (int i = 0; i < 8; i++) {
        free(tasks[i]);
    }

    printf("Thread pool shutdown\n");
}
END_TEST

START_TEST(testThreadPool2) {
    CTLThreadPool pool = CTLThreadPoolCreate("Pool-2", 8);

    CTLTask *task = calloc(1, sizeof(CTLTask));
    task->args.args = NULL;
    task->task = printThreadId2;


    CTLTask *longerTask = calloc(1, sizeof(CTLTask));
    longerTask->args.args = NULL;
    longerTask->task = printThreadId4;

    CTLAwait await = CTLThreadPoolSubmitTaskAsync(pool, task);
    CTLAwait longerAwait = CTLThreadPoolSubmitTaskAsync(pool, longerTask);

    sleep(4);

    // Blocks until this task is complete.
    int *result = CTLAwaitForResult(await);
    int *longerResult = CTLAwaitForResult(longerAwait);
    printf("awaited for result: %d\n", *result);
    printf("awaited for big result: %d\n", *longerResult);

    // Blocks until all tasks are complete.
    CTLThreadPoolShutdownAwait(pool);

    free(task);
    free(longerTask);
}
END_TEST

START_TEST(testThreadPool3) {
    CTLThreadPool pool = CTLThreadPoolCreate("Pool-2", 8);

    CTLTask *task = calloc(1, sizeof(CTLTask));
    task->args.args = NULL;
    task->task = printThreadId2;


    CTLTask *longerTask = calloc(1, sizeof(CTLTask));
    longerTask->args.args = NULL;
    longerTask->task = printThreadId4;

    CTLAwait await = CTLThreadPoolSubmitTaskAsync(pool, task);
    CTLAwait longerAwait = CTLThreadPoolSubmitTaskAsync(pool, longerTask);

    sleep(4);

    // Blocks until this task is complete.
    void *res[2];
    CTLAwaitForAllResults(res, await, longerAwait, NULL);
    printf("awaited for result: %d\n", *(int*)res[0]);
    printf("awaited for big result: %d\n", *(int*)res[1]);

    // Blocks until all tasks are complete.
    CTLThreadPoolShutdownAwait(pool);

    free(task);
    free(longerTask);
}
END_TEST

void test_thread_pool_load_cases(Suite *suite) {
    // test_add_case(suite, testThreadPool);
    test_add_case(suite, testThreadPool2);
}