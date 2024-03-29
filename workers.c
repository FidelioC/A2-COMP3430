#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "queue.h"
#include "globals.h"
#include "scheduler.h"
#include "workers.h"
#include "tasks.h"

void *worker(void *arg) // worker thread
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
                // decide update first arrival response
                decide_task_first_response(node_task);

                // decide IO or no
                decide_task_runtime(node_task);

                pthread_mutex_lock(&total_jobs_received_lock);
                total_jobs_received++;
                printf("Worker %d received task:\n", worker_id);
                print_task(node_task->task);
                printf("Total jobs received by worker: %d\n", total_jobs_received);
                pthread_mutex_unlock(&total_jobs_received_lock);

                // update remaining time of this task
                update_task_length_left(node_task->task);

                // working for the specified run time
                microsleep(node_task->task->task_runtime);

                // update task allotment
                update_task_allotment(node_task->task, node_task->task->task_time_allotment - node_task->task->task_runtime);

                // decide task priority
                decide_task_priority(node_task);

                // decide task destination
                decide_task_destination(node_task);

                printf("Worker %d finished task:\n", worker_id);
                print_task(node_task->task);
            }
        }
        pthread_mutex_unlock(&dispatcher_worker_lock);
    }
    printf("Exiting worker %d thread\n", worker_id);
    pthread_exit(0);
}

void decide_task_first_response(Node *node_task)
{
    if (node_task->task->task_arrival_time != -1)
    {
        update_task_first_response(node_task->task, get_current_time_micro());
    }
}

//------------------------------------------------------
// decide_task_runtime
//
// PURPOSE: decide the task runtime based on the task type, I/O or no
//------------------------------------------------------
void decide_task_runtime(Node *node_task)
{
    if (node_task->task->task_type == 3) // if I/O
    {
        srand(time(NULL));
        int random_number = rand() % MAX_NUMBER + MIN_NUMBER;
        if (random_number <= node_task->task->task_IO_odds) // do IO
        {
            random_number = rand() % QUANTUM_TIME + MIN_NUMBER;
            printf("DOING IO with time slice: %d\n", random_number);
            update_task_runtime(node_task->task, random_number);
            return;
        }
    }

    update_task_runtime(node_task->task, node_task->task->task_quantum_length);
}

void decide_task_priority(Node *node_task)
{
    // if time allotment is empty and priority at least 0 (if less can't go down anymore)
    // move down one priority
    if (node_task->task->task_time_allotment <= 0 && node_task->task->task_priority > 0)
    {
        // move down a priority, allotment has been used, update priority
        update_task_priority(node_task->task, node_task->task->task_priority - 1);
        // reset allotment
        update_task_allotment(node_task->task, ALLOTMENT_TIME);
    }
}

//------------------------------------------------------
// decide_task_destination
//
// PURPOSE: decide the destination of the task, whether to go back to queue
//          or go to destination queue
//------------------------------------------------------
void decide_task_destination(Node *node_task)
{
    // go to one of the queue if task is not finish
    if (!is_task_finished(node_task->task))
    {
        if (node_task->task->task_priority == 4)
        {
            pthread_mutex_lock(&queue_four_lock);
            enqueue(node_task, &queue_four_head, &queue_four_tail);
            pthread_mutex_unlock(&queue_four_lock);
        }
        else if (node_task->task->task_priority == 3)
        {
            pthread_mutex_lock(&queue_three_lock);
            enqueue(node_task, &queue_three_head, &queue_three_tail);
            pthread_mutex_unlock(&queue_three_lock);
        }
        else if (node_task->task->task_priority == 2)
        {
            pthread_mutex_lock(&queue_two_lock);
            enqueue(node_task, &queue_two_head, &queue_two_tail);
            pthread_mutex_unlock(&queue_two_lock);
        }
        else
        {
            pthread_mutex_lock(&queue_one_lock);
            enqueue(node_task, &queue_one_head, &queue_one_tail);
            pthread_mutex_unlock(&queue_one_lock);
        }
    }
    // go to done queue
    else
    {
        pthread_mutex_lock(&queue_done_lock);
        update_task_finish_response(node_task->task, get_current_time_micro());
        enqueue(node_task, &queue_done_head, &queue_done_tail);
        queue_done_size++;
        decide_response_turnaround(node_task);
        pthread_mutex_unlock(&queue_done_lock);
        printf("Task %s is FINISHED\n", node_task->task->task_name);
        // print_queue(queue_done_head, "queue head after added");
    }
}

//------------------------------------------------------
// decide_response_turnaround
//
// PURPOSE: decide the turnaround reponse for each of the task.
//------------------------------------------------------
void decide_response_turnaround(Node *node_task)
{
    if (node_task->task->task_type == 0)
    {
        calculate_response_turnaround(node_task, &total_turnaround_time_type0, &total_response_time_type0);
        total_type0++;
    }
    else if (node_task->task->task_type == 1)
    {
        calculate_response_turnaround(node_task, &total_turnaround_time_type1, &total_response_time_type1);
        total_type1++;
    }
    else if (node_task->task->task_type == 2)
    {
        calculate_response_turnaround(node_task, &total_turnaround_time_type2, &total_response_time_type2);
        total_type2++;
    }
    else
    {
        calculate_response_turnaround(node_task, &total_turnaround_time_type3, &total_response_time_type3);
        total_type3++;
    }
}

void calculate_response_turnaround(Node *node_task, long *turnaround_type, long *response_type)
{
    *response_type = *response_type + (node_task->task->task_first_response - node_task->task->task_arrival_time);
    *turnaround_type = *turnaround_type + (node_task->task->task_finish_time - node_task->task->task_arrival_time);
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

long get_current_time_micro(void)
{
    struct timespec curr_time;

    clock_gettime(CLOCK_REALTIME, &curr_time);

    return (curr_time.tv_sec * USEC_PER_SEC) + (curr_time.tv_nsec / NANOS_PER_USEC);
}
