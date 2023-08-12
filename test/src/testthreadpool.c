#include "ctl/io/logger.h"
#include "ctl/linkedlist.h"
#include "ctl/threads/threadpool.h"
#include "test.h"
#include "testlinkedlist.h"
#include "unistd.h"

static void printThreadId(CTLTaskArgs *args) {
    printf("Thread id: %d start\n", args->threadInfo.id);
    sleep(1);
    printf("Thread id: %d done\n", args->threadInfo.id);
}

START_TEST(testThreadPool) {
    CTLThreadPool pool = CTLThreadPoolCreate("Pool", 8);
    CTLTask *tasks[8];
    for (int i = 0; i < 8; i++) {
        CTLTask *task = calloc(1, sizeof(CTLTask));
        task->args = NULL;
        task->task = printThreadId;
        tasks[i] = task;
        CTLSubmitTask(pool, task);
    }
    CTLThreadPoolShutdownAwait(pool);

    for (int i = 0; i < 8; i++) {
        free(tasks[i]);
    }

    printf("Thread pool shutdown\n");
}
END_TEST

void test_thread_pool_load_cases(Suite *suite) {
    test_add_case(suite, testThreadPool);
}