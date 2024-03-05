#ifndef WORKERS_H
#define WORKERS_H

#include <time.h>

typedef struct timespec timespec;

void *worker(void *arg);
void microsleep(unsigned int usecs);
void decide_task_priority(Node *node_task);
void decide_task_destination(Node *node_task);

#endif
