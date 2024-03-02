#define ALLOTMENT_TIME 200;
#define QUANTUM_TIME 50;

typedef struct
{
    // information from the task file
    char *task_name;    // name of the task
    int task_type;      // 0-short, 1-medium, 2-long, 3-I/O
    long task_length;   // microseconds
    float task_IO_odds; // change percentage to float

    // additional information
    int task_priority;        // 4,3,2,1 -> highest to lowest
    long task_quantum_length; // microseconds
    long task_time_allotment; // microseconds
    long task_arrival_time;   // microseconds
    long task_first_response; // microseconds
    long task_finish_time;    // microseconds
} Task;

Task *create_new_task(char *name, int type, long length, float odds, long arrival_time);

Task *create_new_task(char *name, int type, long length, float odds, long arrival_time)
{
    Task *newTask = (Task *)malloc(sizeof(Task));
    // init information from file
    newTask->task_name = name;
    newTask->task_type = type;
    newTask->task_length = length;
    newTask->task_IO_odds = odds;

    // set default value for additional information
    newTask->task_priority = 4;                    // start from highest
    newTask->task_quantum_length = QUANTUM_TIME;   // init quantum time 50 microsec
    newTask->task_time_allotment = ALLOTMENT_TIME; // init allotment time 200 microsec
    newTask->task_arrival_time = arrival_time;     // get this from the global clock
    newTask->task_first_response = -1;             //-1, still haven't been served
    newTask->task_finish_time = -1;                // default

    return newTask;
}
