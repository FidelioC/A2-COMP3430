#include <pthread.h>

#include "queue.h"
#include "globals.h"
#include "scheduler.h"
#include "dispatcher.h"

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
        elapsed_time = time_spec_to_micro(diff(start_time, curr_time));
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

time_spec diff(time_spec start, time_spec end)
{
    time_spec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0)
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

long time_spec_to_micro(time_spec time)
{
    return time.tv_sec * USEC_PER_SEC + time.tv_nsec / NANOS_PER_USEC;
}
