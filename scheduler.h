#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <time.h>

typedef struct timespec time_spec;

Node *choose_task(void);
bool is_all_queue_empty(void);
void *scheduler_thread_funct(void *ignored);
long calculate_time_elapsed(time_spec start, time_spec current);
// void move_all_jobs(void);
void move_queues_to_queue_four(Node **queue_one_head, Node **queue_one_tail,
                               Node **queue_two_head, Node **queue_two_tail,
                               Node **queue_three_head, Node **queue_three_tail,
                               Node **queue_four_head, Node **queue_four_tail);

void move_jobs_top_queue(Node **source_head, Node **source_tail,
                         Node **destination_head, Node **destination_tail,
                         pthread_mutex_t *source_lock, pthread_mutex_t *destination_lock);
// void move_jobs_top_queue(void);

#endif
