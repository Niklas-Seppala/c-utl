#include "ctl/threads/threadpool.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "ctl/queue.h"

#define MAX_POOL_NAME_LEN 32

struct ctl_thread_pool {
    char name[MAX_POOL_NAME_LEN];
    int nThreads;
    struct thread_info *threads;
    pthread_mutex_t queueMutex;
    pthread_cond_t queueCond;
    CTLQueue taskQueue;
    volatile bool shutdownFlag;
};

static void *__CTLRunThread(void *args) {
    CTLThreadInfo threadInfo = *(CTLThreadInfo *)args;
    CTLThreadPool threadPool = threadInfo.pool;

    while (true) {
        if (threadPool->shutdownFlag) {
            pthread_exit(NULL);
        }

        // Poll the task queue
        pthread_mutex_lock(&threadPool->queueMutex);

        while (!threadPool->shutdownFlag &&
               CTLQueueIsEmpty(threadPool->taskQueue)) {
            pthread_cond_wait(&threadPool->queueCond, &threadPool->queueMutex);
        }
        CTLTask *queuedTask = CTLQueuePoll(threadPool->taskQueue);
        pthread_mutex_unlock(&threadPool->queueMutex);

        if (queuedTask != NULL) {
            // decorate arbitrary arguments with thread info
            // for the caller.
            CTLTaskArgs taskArgs;
            taskArgs.args = queuedTask->args;
            taskArgs.threadInfo = threadInfo;
            // execute
            queuedTask->task(&taskArgs);
        }
    }
}

void CTLSubmitTask(CTLThreadPool pool, CTLTask *task) {
    pthread_mutex_lock(&pool->queueMutex);
    CTLQueuePush(pool->taskQueue, task);
    pthread_mutex_unlock(&pool->queueMutex);
    pthread_cond_signal(&pool->queueCond);
}

void CTLThreadPoolShutdownAwait(CTLThreadPool pool) {
    pool->shutdownFlag = true;
    pthread_cond_broadcast(&pool->queueCond);

    for (int i = 0; i < pool->nThreads; i++) {
        CTLThreadInfo info = pool->threads[i];
        pthread_join(info.thread, NULL);
    }
    pthread_mutex_destroy(&pool->queueMutex);
    pthread_cond_destroy(&pool->queueCond);
    CTLQueueFree(&pool->taskQueue);
    free(pool->threads);
    free(pool);
}

CTLThreadPool CTLThreadPoolCreate(const char *name, int nThreads) {
    assert(strnlen(name, MAX_POOL_NAME_LEN) < MAX_POOL_NAME_LEN);

    CTLThreadPool pool = malloc(sizeof(struct ctl_thread_pool));
    pool->nThreads = nThreads;
    strncpy(pool->name, name, MAX_POOL_NAME_LEN);
    pool->taskQueue = CTLQueueCreate();

    pthread_mutex_init(&(pool->queueMutex), NULL);
    pthread_cond_init(&pool->queueCond, NULL);

    pool->shutdownFlag = false;
    pool->threads = calloc(nThreads, sizeof(CTLThreadInfo));
    for (int i = 0; i < nThreads; i++) {
        CTLThreadInfo *thread_info = pool->threads + i;
        thread_info->pool = pool;
        thread_info->id = i;
        pthread_create(&thread_info->thread, NULL, &__CTLRunThread,
                       pool->threads + i);
    }

    return pool;
}