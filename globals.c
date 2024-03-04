#include "globals.h"

// init general globals
long global_time = 0;
bool is_reading_complete = false;
int total_workers = 0;
// init head and tail queues
Node *queue_four_head = NULL;
Node *queue_four_tail = NULL;

Node *queue_three_head = NULL;
Node *queue_three_tail = NULL;

Node *queue_two_head = NULL;
Node *queue_two_tail = NULL;

Node *queue_one_head = NULL;
Node *queue_one_tail = NULL;

// init dispatcher and worker globals
int total_jobs_received = 0;
bool is_task_available = false;
bool is_terminate_worker = false;
