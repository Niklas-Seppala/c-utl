#if !defined(CTL_THREADS_THREADPOOL_H)
#define CTL_THREADS_THREADPOOL_H
#include "ctl.h"

typedef struct ctl_thread_pool *CTLThreadPool;
typedef struct ctl_await *CTLAwait;

typedef struct thread_info {
    CTLThreadPool pool;
    int id;
    pthread_t thread;
} CTLThreadInfo;

typedef struct ctl_task_args {
    struct thread_info threadInfo;
    void *args;
} CTLTaskArgs;

typedef void *(*CTLThreadTask)(CTLTaskArgs *);

typedef struct ctl_task {
    CTLThreadTask task;
    CTLAwait await;
    CTLTaskArgs args;
} CTLTask;

/**
 * @brief
 *
 * @param pool
 */
void CTLThreadPoolShutdownAwait(CTLThreadPool pool);

/**
 * @brief
 *
 * @param name
 * @param nThreads
 * @return CTLThreadPool
 */
CTLThreadPool CTLThreadPoolCreate(const char *name, int nThreads);

/**
 * @brief
 *
 * @param pool
 * @param task
 */
void CTLThreadPoolSubmitTask(CTLThreadPool pool, CTLTask *task);


/**
 * @brief 
 * 
 * @param token 
 * @return void* 
 */
void *CTLAwaitForResult(CTLAwait token);

/**
 * @brief 
 * 
 * @param results 
 * @param ... 
 */
void CTLAwaitForAllResults(void * results[], ...);

/**
 * @brief 
 * 
 * @param pool 
 * @param task 
 * @return CTLAwait 
 */
CTLAwait CTLThreadPoolSubmitTaskAsync(CTLThreadPool pool, CTLTask *task);

#endif  // CTL_THREADS_THREADPOOL_H
