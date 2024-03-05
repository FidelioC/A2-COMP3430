#include <pthread.h>

#include "queue.h"
#include "globals.h"
#include "scheduler.h"

void *dispatcher(void *ignore)
{
    (void)ignore;
    while (!is_all_queue_empty() || !is_reading_complete)
    {
        // send signal to worker
        pthread_mutex_lock(&dispatcher_worker_lock);
        while (jobs_being_moved)
        {
            is_task_available = false;
            pthread_cond_wait(&dispatcher_cond, &dispatcher_worker_lock);
        }
        is_task_available = true;
        pthread_cond_signal(&worker_cond);
        pthread_mutex_unlock(&dispatcher_worker_lock);
    }
    is_task_available = false;

    // clean any worker
    pthread_mutex_lock(&dispatcher_worker_lock);
    is_terminate_worker = true;
    pthread_cond_broadcast(&worker_cond);
    pthread_mutex_unlock(&dispatcher_worker_lock);

    printf("Exiting dispatcher thread\n");
    pthread_exit(0);
}
