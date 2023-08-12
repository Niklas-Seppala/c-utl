#include "ctl/io/logger.h"
#include "ctl/linkedlist.h"
#include "ctl/threads/threadpool.h"
#include "test.h"
#include "testlinkedlist.h"
#include "unistd.h"

static void *getThreadIdIn2Secs(CTLTaskArgs *args) {
    sleep(10);
    return &args->threadInfo.id;
}


static void *getThreadIdIn4Secs(CTLTaskArgs *args) {
    sleep(20);
    return &args->threadInfo.id;
}

START_TEST(testThreadPool) {
    CTLThreadPool pool = CTLThreadPoolCreate("Pool", 8);
    CTLTask *tasks[8];
    for (int i = 0; i < 8; i++) {
        CTLTask *task = calloc(1, sizeof(CTLTask));
        task->args.args = NULL;
        task->task = getThreadIdIn2Secs;
        tasks[i] = task;
        CTLThreadPoolSubmitTask(pool, task);
    }
    CTLThreadPoolAwaitShutdown(pool);

    for (int i = 0; i < 8; i++) {
        free(tasks[i]);
    }

    printf("Thread pool shutdown\n");
}
END_TEST

START_TEST(testThreadPool2) {
    CTLThreadPool pool = CTLThreadPoolCreate("Pool-2", 8);

    CTLTask task;
    task.args.args = NULL;
    task.task = getThreadIdIn2Secs;

    CTLTask longerTask;
    longerTask.args.args = NULL;
    longerTask.task = getThreadIdIn4Secs;

    CTLAwaitToken token1 = CTLThreadPoolRunAsync(pool, &task);
    CTLAwaitToken token2 = CTLThreadPoolRunAsync(pool, &longerTask);

    // Blocks until both resolve.
    int *result = CTLAwaitForResult(token1);
    int *longerResult = CTLAwaitForResult(token2);

    printf("result: %d\n", *result);
    printf("big result: %d\n", *longerResult);

    // Blocks until all tasks are complete, and pool is disposed.
    CTLThreadPoolAwaitShutdown(pool);
}
END_TEST

START_TEST(testThreadPool3) {
    CTLThreadPool pool = CTLThreadPoolCreate("Pool-2", 8);

    CTLTask *task = calloc(1, sizeof(CTLTask));
    task->args.args = NULL;
    task->task = getThreadIdIn2Secs;

    CTLTask *longerTask = calloc(1, sizeof(CTLTask));
    longerTask->args.args = NULL;
    longerTask->task = getThreadIdIn4Secs;

    CTLAwaitToken token1 = CTLThreadPoolRunAsync(pool, task);
    CTLAwaitToken token2 = CTLThreadPoolRunAsync(pool, longerTask);

    // Blocks until both tasks resolve.
    void *collect[2];
    CTLAwaitForAll(collect, token1, token2, NULL);

    printf("awaited for result: %d\n", *(int*)collect[0]);
    printf("awaited for big result: %d\n", *(int*)collect[1]);

    // Blocks until all tasks are complete.
    CTLThreadPoolAwaitShutdown(pool);

    free(task);
    free(longerTask);
}
END_TEST

void test_thread_pool_load_cases(Suite *suite) {
    // test_add_case(suite, testThreadPool);
    test_add_case(suite, testThreadPool2);
}