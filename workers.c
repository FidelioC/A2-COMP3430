#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "queue.h"
#include "globals.h"
#include "scheduler.h"
#include "workers.h"

void *worker(void *arg)
{
    Node *node_task;
    int worker_id = *((int *)arg);
    printf("worker %d created\n", worker_id);
    while (!is_reading_complete || !is_all_queue_empty() || !is_terminate_worker)
    {
        pthread_mutex_lock(&dispatcher_worker_lock);

        // wait till task become available
        while (!is_task_available && !is_terminate_worker)
        {
            printf("Waiting for job availability in worker %d\n", worker_id);
            pthread_cond_wait(&worker_cond, &dispatcher_worker_lock);
        }

        if (!is_terminate_worker)
        {
            // get task from mlfq
            node_task = choose_task();

            // do work
            if (node_task != NULL)
            {
                pthread_mutex_lock(&total_jobs_received_lock);
                total_jobs_received++;
                printf("Worker %d received task:\n", worker_id);
                print_task(node_task->task);
                printf("Total jobs received by worker: %d\n", total_jobs_received);
                pthread_mutex_unlock(&total_jobs_received_lock);

                // working for the specified run time
                microsleep(node_task->task->task_runtime);
            }
        }
        pthread_mutex_unlock(&dispatcher_worker_lock);
    }
    printf("Exiting worker %d thread\n", worker_id);
    pthread_exit(0);
}

void microsleep(unsigned int usecs)
{
    long seconds = usecs / USEC_PER_SEC;
    long nanos = (usecs % USEC_PER_SEC) * NANOS_PER_USEC;
    struct timespec t = {.tv_sec = seconds, .tv_nsec = nanos};
    int ret;
    do
    {
        ret = nanosleep(&t, &t);
        // need to loop, `nanosleep` might return before sleeping
        // for the complete time (see `man nanosleep` for details)
    } while (ret == -1 && (t.tv_sec || t.tv_nsec));
}
