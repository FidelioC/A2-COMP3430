#include "globals.h"

// init general globals
long global_time_S = 0;
bool is_reading_complete = false;
int total_workers = 0;

// init mlfq globals
Node *queue_four_head = NULL;
Node *queue_four_tail = NULL;

Node *queue_three_head = NULL;
Node *queue_three_tail = NULL;

Node *queue_two_head = NULL;
Node *queue_two_tail = NULL;

Node *queue_one_head = NULL;
Node *queue_one_tail = NULL;

int queue_four_size = 0;
int queue_three_size = 0;
int queue_two_size = 0;
int queue_one_size = 0;
bool jobs_being_moved = false;

// init dispatcher and worker globals
int total_jobs_received = 0;
bool is_task_available = false;
bool is_terminate_worker = false;

// done globals
Node *queue_done_head = NULL;
Node *queue_done_tail = NULL;
int queue_done_size = 0;

long total_response_time_type0 = 0;
long total_turnaround_time_type0 = 0;
int total_type0 = 0;

long total_response_time_type1 = 0;
long total_turnaround_time_type1 = 0;
int total_type1 = 0;

long total_response_time_type2 = 0;
long total_turnaround_time_type2 = 0;
int total_type2 = 0;

long total_response_time_type3 = 0;
long total_turnaround_time_type3 = 0;
int total_type3 = 0;
