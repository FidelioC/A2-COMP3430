#ifndef TASKS_H
#define TASKS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    // information from the task file
    char *task_name;  // name of the task
    int task_type;    // 0-short, 1-medium, 2-long, 3-I/O
    long task_length; // microseconds
    int task_IO_odds; // percentage IO

    // additional information
    int task_priority;        // 4,3,2,1 -> highest to lowest
    long task_runtime;        // runtime of a task
    long task_quantum_length; // microseconds
    long task_time_allotment; // microseconds
    long task_length_left;    // remaining length microseconds
    long task_arrival_time;   // microseconds
    long task_first_response; // microseconds
    long task_finish_time;    // microseconds
} Task;

Task *create_new_task(char *name, int type, long length, float odds, long arrival_time);
void update_task_runtime(Task *task, long runtime);
void update_task_allotment(Task *task, long allotment);
void update_task_quantum(Task *task, long quantum);
void update_task_priority(Task *task, int new_priority);
void update_task_length_left(Task *task);
bool is_task_finished(Task *task);
void print_task(Task *task);

#endif
