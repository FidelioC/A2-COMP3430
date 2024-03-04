#include <pthread.h>
#include <unistd.h>

#include "queue.h"
#include "globals.h"
#include "scheduler.h"

void *worker(void *arg)
{
    Node *node_task;
    int worker_id = *((int *)arg);
    printf("worker %d created\n", worker_id);
    while (1)
    {
        pthread_mutex_lock(&dispatcher_worker_lock);

        // wait till task become available
        while (!task_available)
        {
            printf("Waiting for job availability\n");
            pthread_cond_wait(&worker_cond, &dispatcher_worker_lock);
        }

        // get task from mlfq
        node_task = choose_task();

        // do work
        if (node_task != NULL)
        {
            // usleep(10000);
            pthread_mutex_lock(&total_jobs_received_lock);
            total_jobs_received++;
            pthread_mutex_unlock(&total_jobs_received_lock);
            printf("Worker %d received task: %s\n", worker_id, node_task->task->task_name);
            printf("\nTotal jobs received by worker: %d\n\n", total_jobs_received);
        }
        pthread_cond_signal(&dispatcher_cond);
        pthread_mutex_unlock(&dispatcher_worker_lock);
    }
    printf("Exiting worker %d thread\n", worker_id);
    pthread_exit(0);
}
