#include "tasks.h"

Task *create_new_task(char *name, int type, long length, float odds, long arrival_time)
{
    Task *newTask = (Task *)malloc(sizeof(Task));
    // init information from file
    newTask->task_name = name;
    newTask->task_type = type;
    newTask->task_length = length;
    newTask->task_IO_odds = odds;

    // set default value for additional information
    newTask->task_priority = 4;                // start from highest
    newTask->task_runtime = -1;                // init task runtime
    newTask->task_length_left = length;        // init length = original length
    newTask->task_arrival_time = arrival_time; // get this from the global clock
    newTask->task_first_response = -1;         //-1, still haven't been served
    newTask->task_finish_time = -1;            // default

    return newTask;
}

void print_task(Task *task)
{
    printf("Task details:\n");
    printf("Task Name: %s\n", task->task_name);
    printf("Task Type: %d\n", task->task_type);
    printf("Task Length: %ld microseconds\n", task->task_length);
    printf("Task IO Odds: %f\n", task->task_IO_odds);
    printf("Task Priority: %d\n", task->task_priority);
    printf("Task Runtime: %ld microseconds\n", task->task_runtime);
    printf("Task Length Left: %ld microseconds\n", task->task_length_left);
    printf("Task Arrival Time: %ld microseconds\n", task->task_arrival_time);
    printf("Task First Response: %ld microseconds\n", task->task_first_response);
    printf("Task Finish Time: %ld microseconds\n", task->task_finish_time);
    printf("\n");
}
