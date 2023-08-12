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

struct ctl_await {
    pthread_cond_t waitCond;
    pthread_mutex_t taskMutex;
    volatile bool ready;
    void *result;
};

/**
 * @brief 
 * 
 * @return CTLAwait 
 */
static CTLAwaitToken __CTLAwaitCreate(void);

/**
 * @brief 
 * 
 * @param await 
 * @param result 
 */
static void __CTLAwaitPostResult(CTLAwaitToken await, void *result);

/**
 * @brief 
 * 
 * @param args 
 * @return void* 
 */
static void *__CTLRunThread(void *args);

void CTLThreadPoolSubmitTask(CTLThreadPool pool, CTLTask *task) {
    pthread_mutex_lock(&pool->queueMutex);
    CTLQueuePush(pool->taskQueue, task);
    pthread_mutex_unlock(&pool->queueMutex);
    pthread_cond_signal(&pool->queueCond);
}

void CTLAwaitForAll(void * results[], ...) {
    va_list args;
    va_start(args, results);

    int i = 0;
    CTLAwaitToken await = NULL;
    while ((await = va_arg(args, CTLAwaitToken)) != NULL) {
        void *result = CTLAwaitForResult(await);
        results[i++] = result;
    }
    va_end(args);
}

void *CTLAwaitForResult(CTLAwaitToken await) {
    while (!await->ready) {
        pthread_cond_wait(&await->waitCond, &await->taskMutex);
    }
    pthread_mutex_destroy(&await->taskMutex);
    pthread_cond_destroy(&await->waitCond);

    void *result = await->result;
    free(await);
    return result;
}

CTLAwaitToken CTLThreadPoolRunAsync(CTLThreadPool pool, CTLTask *task) {
    task->await = __CTLAwaitCreate();
    pthread_mutex_lock(&pool->queueMutex);
    CTLQueuePush(pool->taskQueue, task);
    pthread_mutex_unlock(&pool->queueMutex);
    pthread_cond_signal(&pool->queueCond);
    return task->await;
}

void CTLThreadPoolAwaitShutdown(CTLThreadPool pool) {
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

static CTLAwaitToken __CTLAwaitCreate(void) {
    CTLAwaitToken token = malloc(sizeof(struct ctl_await));
    if (token == NULL) {
        return NULL;
    }
    token->result = NULL;
    token->ready = false;

    pthread_mutex_init(&token->taskMutex, NULL);
    pthread_cond_init(&token->waitCond, NULL);
    
    return token;
}

static void __CTLAwaitPostResult(CTLAwaitToken await, void *result) {
    await->result = result;
    await->ready = true;
    pthread_cond_signal(&await->waitCond);
}


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
            queuedTask->args.threadInfo = threadInfo;
            
            // execute
            void *result = queuedTask->task(&queuedTask->args);

            // post result, if it's await task
            if (queuedTask->await != NULL) {
                __CTLAwaitPostResult(queuedTask->await, result);
            }
        }
    }
}