#include <time.h>
#include <stdio.h>

#include "queue.h"
#include "globals.h"
#include "tasks.h"
#include "scheduler.h"

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

long calculate_time_elapsed(time_spec start, time_spec current)
{
    long sec_diff = current.tv_sec - start.tv_sec;
    long nsec_diff = current.tv_nsec - start.tv_nsec;

    return (sec_diff * USEC_PER_SEC) + (nsec_diff / NANOS_PER_USEC);
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
