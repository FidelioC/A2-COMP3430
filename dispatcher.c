#include <pthread.h>

#include "queue.h"
#include "globals.h"
#include "scheduler.h"

void *dispatcher(void *ignore)
{
    (void)ignore;
    time_spec start_time;
    time_spec curr_time;
    long elapsed_time;
    clock_gettime(CLOCK_REALTIME, &start_time);
    while (!is_all_queue_empty() || !is_reading_complete)
    {
        clock_gettime(CLOCK_REALTIME, &curr_time);
        elapsed_time = calculate_time_elapsed(start_time, curr_time);
        // printf("Elapsed time is %ld\n", elapsed_time);
        if (elapsed_time >= global_time_S)
        {
            printf("Elapsed time has passed %ld\n", elapsed_time);

            // task not available
            pthread_mutex_lock(&dispatcher_worker_lock);
            is_task_available = false;
            pthread_mutex_unlock(&dispatcher_worker_lock);

            // reset start time
            clock_gettime(CLOCK_REALTIME, &start_time);

            // increase all priority
            move_queues_to_queue_four(&queue_one_head, &queue_one_tail,
                                      &queue_two_head, &queue_two_tail,
                                      &queue_three_head, &queue_three_tail,
                                      &queue_four_head, &queue_four_tail);
        }
        else
        {
            // send signal to worker
            pthread_mutex_lock(&dispatcher_worker_lock);
            // while (jobs_being_moved)
            // {
            //     is_task_available = false;
            //     pthread_cond_wait(&dispatcher_cond, &dispatcher_worker_lock);
            // }
            is_task_available = true;
            pthread_cond_signal(&worker_cond);
            pthread_mutex_unlock(&dispatcher_worker_lock);
        }
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
