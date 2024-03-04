#ifndef SCHEDULER_H
#define SCHEDULER_H

Node *choose_task(void);
bool is_all_queue_empty(void);
void *scheduler_thread_funct(void *ignored);

#endif
