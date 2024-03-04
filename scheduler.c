#include "queue.h"
#include "globals.h"
#include "tasks.h"
#include "scheduler.h"

// TODO:
//  functions:
//   - dequeue and return which job to go to the dispatched queue
//   - move all jobs to top most queue after some period time, reset its time allotment

void *scheduler_thread_funct(void *ignored)
{
    (void)ignored; // ignore this param
    Node *chosen_task;
    while (!is_reading_complete && !is_all_queue_empty())
    {
        chosen_task = choose_task();
        if (chosen_task != NULL && chosen_task->task != NULL)
        {
            printf("chosen task: %s\n\n", chosen_task->task->task_name);
        }
    }

    printf("Exiting scheduler thread\n");
    pthread_exit(0);
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
    if (!is_queue_empty(&queue_four_head, &queue_four_tail))
    {
        pthread_mutex_lock(&queue_four_lock);
        chosen_task = dequeue(&queue_four_head);
        pthread_mutex_unlock(&queue_four_lock);
    }
    else if (!is_queue_empty(&queue_three_head, &queue_three_tail))
    {
        pthread_mutex_lock(&queue_three_lock);
        chosen_task = dequeue(&queue_three_head);
        pthread_mutex_unlock(&queue_three_lock);
    }
    else if (!is_queue_empty(&queue_two_head, &queue_two_tail))
    {
        pthread_mutex_lock(&queue_two_lock);
        chosen_task = dequeue(&queue_two_head);
        pthread_mutex_unlock(&queue_two_lock);
    }
    else
    {
        pthread_mutex_lock(&queue_one_lock);
        chosen_task = dequeue(&queue_one_head);
        pthread_mutex_unlock(&queue_one_lock);
    }
    return chosen_task;
}

bool is_all_queue_empty(void)
{
    return (is_queue_empty(&queue_four_head, &queue_four_tail) == true) &&
           (is_queue_empty(&queue_three_head, &queue_three_tail) == true) &&
           (is_queue_empty(&queue_two_head, &queue_two_tail) == true) &&
           (is_queue_empty(&queue_one_head, &queue_one_tail) == true);
}
