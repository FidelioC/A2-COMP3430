#ifndef WORKERS_H
#define WORKERS_H

#include <time.h>

#define MIN_NUMBER 0
#define MAX_NUMBER 100

void *worker(void *arg);
void microsleep(unsigned int usecs);
void decide_task_priority(Node *node_task);
void decide_task_destination(Node *node_task);
void decide_task_runtime(Node *node_task);
long get_current_time_micro(void);
void decide_task_first_response(Node *node_task);
void decide_response_turnaround(Node *node_task);
void calculate_response_turnaround(Node *node_task, long *turnaround_type, long *response_type);

#endif
