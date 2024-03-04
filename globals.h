#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include "queue.h"

#define ALLOTMENT_TIME 200;
#define QUANTUM_TIME 50;

// global tracking time in microseconds
extern long global_time;

// file reading boolean
extern bool is_reading_complete;

// total workers
extern int total_workers;

// === SCHEDULER GLOBALS ===
// locks for scheduler queue
pthread_mutex_t queue_four_lock;
pthread_mutex_t queue_three_lock;
pthread_mutex_t queue_two_lock;
pthread_mutex_t queue_one_lock;

// Define global variables for the ready queues
extern Node *queue_four_head;
extern Node *queue_four_tail;

extern Node *queue_three_head;
extern Node *queue_three_tail;

extern Node *queue_two_head;
extern Node *queue_two_tail;

extern Node *queue_one_head;
extern Node *queue_one_tail;

//===== dispatcher and workers globals ======
extern int total_jobs_received;
extern bool is_task_available;
extern bool is_terminate_worker;

// condition vars
pthread_cond_t worker_cond;

// locks
pthread_mutex_t total_jobs_received_lock;
pthread_mutex_t dispatcher_worker_lock;
pthread_mutex_t queue_dispatcher_lock;

#endif
