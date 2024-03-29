#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include "queue.h"

#define ALLOTMENT_TIME 200
#define QUANTUM_TIME 50
#define NANOS_PER_USEC 1000
#define USEC_PER_SEC 1000000

// global tracking time in microseconds
extern long global_time_S;

// file reading boolean
extern bool is_reading_complete;

// total workers
extern int total_workers;

// === SCHEDULER GLOBALS ===
// locks for scheduler queue
extern pthread_mutex_t queue_four_lock;
extern pthread_mutex_t queue_three_lock;
extern pthread_mutex_t queue_two_lock;
extern pthread_mutex_t queue_one_lock;
extern pthread_mutex_t move_jobs_lock;

// define global variables for the ready queues
extern Node *queue_four_head;
extern Node *queue_four_tail;

extern Node *queue_three_head;
extern Node *queue_three_tail;

extern Node *queue_two_head;
extern Node *queue_two_tail;

extern Node *queue_one_head;
extern Node *queue_one_tail;

extern int queue_four_size;
extern int queue_three_size;
extern int queue_two_size;
extern int queue_one_size;
extern bool jobs_being_moved;

//===== dispatcher and workers globals ======
extern int total_jobs_received;
extern bool is_task_available;
extern bool is_terminate_worker;

// condition vars
extern pthread_cond_t worker_cond;
extern pthread_cond_t dispatcher_cond;

// locks
extern pthread_mutex_t total_jobs_received_lock;
extern pthread_mutex_t dispatcher_worker_lock;
extern pthread_mutex_t queue_dispatcher_lock;

// ===== done section globals =====
extern Node *queue_done_head;
extern Node *queue_done_tail;

extern pthread_mutex_t queue_done_lock;

extern int queue_done_size;

extern long total_response_time_type0;
extern long total_turnaround_time_type0;
extern int total_type0;

extern long total_response_time_type1;
extern long total_turnaround_time_type1;
extern int total_type1;

extern long total_response_time_type2;
extern long total_turnaround_time_type2;
extern int total_type2;

extern long total_response_time_type3;
extern long total_turnaround_time_type3;
extern int total_type3;

#endif
