#include <time.h>
#include <stdio.h>

#include "queue.h"
#include "globals.h"
#include "tasks.h"
#include "scheduler.h"

// TODO:
//  functions:
//   - dequeue and return which job to go to the dispatched queue (done)
//   - move all jobs to top most queue after some period time, reset its time allotment

void *scheduler_thread_funct(void *ignored)
{
    (void)ignored; // ignore this param
    time_spec start_time;
    time_spec curr_time;
    long elapsed_time;
    clock_gettime(CLOCK_REALTIME, &start_time);
    while (!is_reading_complete || !is_all_queue_empty())
    {
        clock_gettime(CLOCK_REALTIME, &curr_time);
        elapsed_time = calculate_time_elapsed(start_time, curr_time);
        // printf("Elapsed time is %ld\n", elapsed_time);
        if (elapsed_time >= global_time_S)
        {
            printf("Elapsed time has passed %ld\n", elapsed_time);

            jobs_being_moved = true;
            // reset start time
            clock_gettime(CLOCK_REALTIME, &start_time);

            // increase all priority
            // pthread_mutex_lock(&queue_four_lock);
            // move_queues_to_queue_four(&queue_one_head, &queue_one_tail,
            //                           &queue_two_head, &queue_two_tail,
            //                           &queue_three_head, &queue_three_tail,
            //                           &queue_four_head, &queue_four_tail);
            // pthread_mutex_unlock(&queue_four_lock);
            // move_all_jobs();
            // jobs_being_moved = false;
            // pthread_cond_signal(&dispatcher_cond);
        }
    }

    printf("Exiting scheduler thread\n");
    pthread_exit(0);
}

void move_queues_to_queue_four(Node **queue_one_head, Node **queue_one_tail,
                               Node **queue_two_head, Node **queue_two_tail,
                               Node **queue_three_head, Node **queue_three_tail,
                               Node **queue_four_head, Node **queue_four_tail)
{
    printf("move queues to top queue\n");
    move_jobs_top_queue(queue_three_head, queue_three_tail, queue_four_head, queue_four_tail, &queue_three_lock, &queue_four_lock);
    move_jobs_top_queue(queue_two_head, queue_two_tail, queue_four_head, queue_four_tail, &queue_two_lock, &queue_four_lock);
    move_jobs_top_queue(queue_one_head, queue_one_tail, queue_four_head, queue_four_tail, &queue_one_lock, &queue_four_lock);
}

void move_jobs_top_queue(Node **source_head, Node **source_tail,
                         Node **destination_head, Node **destination_tail,
                         pthread_mutex_t *source_lock, pthread_mutex_t *destination_lock)
{
    pthread_mutex_lock(source_lock);
    while (*source_head != NULL)
    {
        Node *chosen_task = dequeue(source_head, source_tail);
        printf("Moving Task from queue: %s\n", chosen_task->task->task_name);
        update_task_priority(chosen_task->task, 4);
        pthread_mutex_lock(destination_lock);
        enqueue(chosen_task, destination_head, destination_tail);
        pthread_mutex_unlock(destination_lock);
    }
    pthread_mutex_unlock(source_lock);
}
// void move_all_jobs(void)
// {
//     // pthread_mutex_lock(&move_jobs_lock);
//     pthread_mutex_lock(&queue_four_lock);

//     pthread_mutex_lock(&queue_three_lock);
//     move_jobs_top_queue(queue_three_head, &queue_three_lock, "queue_3");
//     pthread_mutex_unlock(&queue_three_lock);

//     pthread_mutex_lock(&queue_two_lock);
//     move_jobs_top_queue(queue_two_head, &queue_two_lock, "queue_2");
//     pthread_mutex_unlock(&queue_two_lock);

//     pthread_mutex_lock(&queue_one_lock);
//     move_jobs_top_queue(queue_one_head, &queue_one_lock, "queue_1");
//     pthread_mutex_unlock(&queue_one_lock);

//     pthread_mutex_unlock(&queue_four_lock);
//     // pthread_mutex_unlock(&move_jobs_lock);
// }

// void move_jobs_top_queue(void)
// {
//     Node *chosen_task;
//     printf("Queue four before: \n");
//     print_queue(queue_four_head, "queue four");
//     pthread_mutex_lock(&queue_three_lock);
//     while (queue_three_head != NULL)
//     {
//         chosen_task = dequeue(&queue_three_head); // dequeue from current queue
//         if (chosen_task != NULL)
//         {
//             printf("Moving Task from queue three: %s\n", chosen_task->task->task_name);
//             update_task_priority(chosen_task->task, 4);               // update priority
//             enqueue(chosen_task, &queue_four_head, &queue_four_tail); // move to top queue
//         }
//     }
//     pthread_mutex_unlock(&queue_three_lock);

//     pthread_mutex_lock(&queue_two_lock);
//     while (!is_queue_empty(&queue_two_head))
//     {
//         printf("Moving Task from queue two: %s\n", queue_two_head->task->task_name);

//         chosen_task = dequeue(&queue_two_head);                   // dequeue from current queue
//         update_task_priority(chosen_task->task, 4);               // update priority
//         enqueue(chosen_task, &queue_four_head, &queue_four_tail); // move to top queue
//     }
//     pthread_mutex_unlock(&queue_two_lock);

//     pthread_mutex_lock(&queue_one_lock);
//     while (!is_queue_empty(&queue_one_head))
//     {
//         printf("Moving Task from queue one: %s\n", queue_one_head->task->task_name);

//         chosen_task = dequeue(&queue_one_head);                   // dequeue from current queue
//         update_task_priority(chosen_task->task, 4);               // update priority
//         enqueue(chosen_task, &queue_four_head, &queue_four_tail); // move to top queue
//     }
//     pthread_mutex_unlock(&queue_one_lock);
//     printf("Queue four after: \n");
//     print_queue(queue_four_head, "queue four");
// }
// void move_jobs_top_queue(Node *head, pthread_mutex_t *queue_source_lock, char *queue_name)
// {
//     (void)queue_source_lock;
//     printf("Moving jobs to top queue from %s\n", queue_name);
//     Node *chosen_task = NULL;
//     while (!is_queue_empty(&head))
//     {
//         printf("Moving Task: %s\n", head->task->task_name);

//         // pthread_mutex_lock(queue_source_lock);
//         chosen_task = dequeue(&head);               // dequeue from current queue
//         update_task_priority(chosen_task->task, 4); // update priority
//         // pthread_mutex_unlock(queue_source_lock);

//         // pthread_mutex_lock(&queue_four_lock);
//         enqueue(chosen_task, &queue_four_head, &queue_four_tail); // move to top queue
//         // pthread_mutex_unlock(&queue_four_lock);

//         // print_task(curr->task);
//     }
// }

long calculate_time_elapsed(time_spec start, time_spec current)
{
    long sec_diff = current.tv_sec - start.tv_sec;
    long nsec_diff = current.tv_nsec - start.tv_nsec;

    return sec_diff * USEC_PER_SEC + nsec_diff / NANOS_PER_USEC;
}

Node *choose_task(void)
{
    // this includes Rule 1 and Rule 2:
    //  Rule 1: it will choose the task base on the queue level (priority automatically implied)
    //  Rule 2: the round robin approach is implied on the queue itself,
    //          when task is chosen, it will be dequeued from the front queue,
    //          on the other hand, when task hasn't finished but still on the same priority,
    //          it will be enqueued at the back of the same queue.
    //          this automatically implied the round robin approach.
    Node *chosen_task;
    if (!is_queue_empty(&queue_four_head))
    {
        pthread_mutex_lock(&queue_four_lock);
        chosen_task = dequeue(&queue_four_head, &queue_four_tail);
        pthread_mutex_unlock(&queue_four_lock);
    }
    else if (!is_queue_empty(&queue_three_head))
    {
        pthread_mutex_lock(&queue_three_lock);
        chosen_task = dequeue(&queue_three_head, &queue_three_tail);
        pthread_mutex_unlock(&queue_three_lock);
    }
    else if (!is_queue_empty(&queue_two_head))
    {
        pthread_mutex_lock(&queue_two_lock);
        chosen_task = dequeue(&queue_two_head, &queue_two_tail);
        pthread_mutex_unlock(&queue_two_lock);
    }
    else
    {
        pthread_mutex_lock(&queue_one_lock);
        chosen_task = dequeue(&queue_one_head, &queue_one_tail);
        pthread_mutex_unlock(&queue_one_lock);
    }
    return chosen_task;
}

bool is_all_queue_empty(void)
{
    return (is_queue_empty(&queue_four_head)) &&
           (is_queue_empty(&queue_three_head)) &&
           (is_queue_empty(&queue_two_head)) &&
           (is_queue_empty(&queue_one_head));
}
