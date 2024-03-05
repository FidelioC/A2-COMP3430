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

#endif
