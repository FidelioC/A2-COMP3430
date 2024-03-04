#include <pthread.h>

#include "queue.h"
#include "globals.h"
#include "scheduler.h"

void *dispatcher(void *ignore)
{
    (void)ignore;
    while (!is_queue_empty(&queue_dispatcher_head) || !is_reading_complete || !is_all_queue_empty())
    {
        // send signal to worker
        pthread_mutex_lock(&dispatcher_worker_lock);
        print_queue(queue_dispatcher_head, "dispatcher queue");
        pthread_cond_signal(&worker_cond);
        pthread_mutex_unlock(&dispatcher_worker_lock);
    }
    printf("Exiting dispatcher thread\n");
    pthread_exit(0);
}
