#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/queue.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>

#include "tasks.h"
#include "queue.h"
#include "globals.h"
#include "scheduler.h"
#include "workers.h"
#include "dispatcher.h"

#define MAX_TASK_ENTRY 4

void *read_file(void *args);
char **split_to_array(char *line);
void print_array(char **array, int count);
void add_task(char **splitted_array);
void init_locks(void);
void create_worker_threads(int total_workers, pthread_t *worker_threads);
void print_done_result(void);

typedef struct
{
    FILE *file;
    bool init_read;
    long file_position;
} ReadFileParams;

int main(int argc, char *argv[])
{
    // cmd argument check
    if (argc != 4)
    {
        printf("Please provide the correct argument. Need 3 inputs arg <CPU> <S> <file_name.txt>.\n");
        exit(EXIT_FAILURE);
    }
    // init variables
    ReadFileParams args;
    pthread_t reader_thread;
    pthread_t dispatcher_thread;
    char *file_name = argv[3];
    total_workers = atoi(argv[1]);
    global_time_S = strtol(argv[2], NULL, 10); // in microseconds
    pthread_t worker_threads[total_workers];

    // init param
    args.file = fopen(file_name, "r");
    args.init_read = true;
    args.file_position = 0;

    // init load tasks
    read_file(&args);

    // finish init tasks
    args.init_read = false;

    // init locks
    init_locks();

    // init threads
    pthread_create(&reader_thread, NULL, read_file, &args);
    pthread_create(&dispatcher_thread, NULL, dispatcher, NULL);
    create_worker_threads(total_workers, worker_threads);

    // wait for threads
    pthread_join(reader_thread, NULL);
    pthread_join(dispatcher_thread, NULL);
    for (int i = 0; i < total_workers; i++)
    {
        pthread_join(worker_threads[i], NULL);
    }
    print_done_result();

    fclose(args.file);

    return EXIT_SUCCESS;
}

void print_done_result(void)
{
    printf("QUEUE DONE SIZE %d\n", queue_done_size);
    print_queue(queue_done_head, "DONE QUEUE: ");

    // printf("Total turnaround type 0 %ld\n", total_turnaround_time_type0);
    // printf("Total turnaround type 1 %ld\n", total_turnaround_time_type1);
    // printf("Total turnaround type 2 %ld\n", total_turnaround_time_type2);
    // printf("Total turnaround type 3 %ld\n", total_turnaround_time_type3);
    // printf("\n");
    // printf("Total response type 0 %ld\n", total_response_time_type0);
    // printf("Total response type 1 %ld\n", total_response_time_type1);
    // printf("Total response type 2 %ld\n", total_response_time_type2);
    // printf("Total response type 3 %ld\n", total_response_time_type3);
    // printf("\n");
    // printf("Total type 0 %d\n", total_type0);
    // printf("Total type 1 %d\n", total_type1);
    // printf("Total type 2 %d\n", total_type2);
    // printf("Total type 3 %d\n", total_type3);

    printf("Using mlfq with %d CPUs.\n\n", total_workers);
    printf("Average turnaround time per type:\n");
    printf("\t Type 0: %ld usec \n", (total_turnaround_time_type0 / total_type0));
    printf("\t Type 1: %ld usec\n", (total_turnaround_time_type1 / total_type1));
    printf("\t Type 2: %ld usec\n", (total_turnaround_time_type2 / total_type2));
    printf("\t Type 3: %ld usec\n", (total_turnaround_time_type3 / total_type3));

    printf("Average response time per type:\n");
    printf("\t Type 0: %ld usec\n", (total_response_time_type0 / total_type0));
    printf("\t Type 1: %ld usec\n", (total_response_time_type1 / total_type1));
    printf("\t Type 2: %ld usec\n", (total_response_time_type2 / total_type2));
    printf("\t Type 3: %ld usec\n", (total_response_time_type3 / total_type3));
}

void create_worker_threads(int total_workers, pthread_t *worker_threads)
{
    for (int i = 0; i < total_workers; i++)
    {
        int *worker_id = (int *)malloc(sizeof(int));
        *worker_id = i;
        printf("creating worker %d\n", *worker_id);
        pthread_create(&worker_threads[i], NULL, worker, (void *)worker_id);
    }
}

void init_locks(void)
{
    // init locks
    pthread_mutex_init(&queue_four_lock, NULL);
    pthread_mutex_init(&queue_three_lock, NULL);
    pthread_mutex_init(&queue_two_lock, NULL);
    pthread_mutex_init(&queue_one_lock, NULL);
    pthread_mutex_init(&queue_dispatcher_lock, NULL);
    pthread_mutex_init(&total_jobs_received_lock, NULL);
    pthread_mutex_init(&dispatcher_worker_lock, NULL);
    pthread_mutex_init(&queue_dispatcher_lock, NULL);
    pthread_mutex_init(&queue_done_lock, NULL);
    pthread_mutex_init(&move_jobs_lock, NULL);

    // init cond
    pthread_cond_init(&worker_cond, NULL);
    pthread_cond_init(&dispatcher_cond, NULL);
}

void add_task(char **splitted_array)
{
    char *name = splitted_array[0];     // task name
    int type = atoi(splitted_array[1]); // task type or delay time
    long length;                        // task length
    float odds;                         // io odds

    // add task to the top of ready queue
    if (strcmp(name, "DELAY") != 0)
    {
        printf("adding task: %s\n\n", name);
        length = strtol(splitted_array[2], NULL, 10); // 10 = base 10
        odds = atoi(splitted_array[3]);
        Task *new_task = create_new_task(name, type, length, odds, get_current_time_micro());
        Node *new_node = (Node *)malloc(sizeof(Node));
        new_node->task = new_task;

        // Rule 3: when job enters the system, placed at the highest priority
        pthread_mutex_lock(&queue_four_lock);
        enqueue(new_node, &queue_four_head, &queue_four_tail);
        pthread_mutex_unlock(&queue_four_lock);
    }
    else
    {
        microsleep((type * NANOS_PER_USEC)); // convert to microseconds
    }
}

void *read_file(void *args)
{
    ReadFileParams *params = (ReadFileParams *)args;
    FILE *file = params->file;
    bool init_read = params->init_read;
    long file_position = params->file_position;
    char *line = NULL;
    char **splitted_array;
    size_t length = 0;

    fseek(file, file_position, SEEK_SET);

    while (getline(&line, &length, file) != -1)
    {
        printf("line is: %s", line);
        splitted_array = split_to_array(line);
        if (init_read && strcmp(splitted_array[0], "DELAY") == 0)
        {
            params->file_position = ftell(file);
            microsleep(atoi(splitted_array[1]));
            return 0;
        }
        add_task(splitted_array);
    }
    free(line);
    is_reading_complete = true;

    printf("Exiting reading thread\n");
    pthread_exit(0);
}

char **split_to_array(char *line)
{
    // someone said that using strtok_r is better when we're doing multi threading
    // thats why I am using strtok_r rather than strtok
    // https://stackoverflow.com/questions/15472299/split-string-into-tokens-and-save-them-in-an-array
    char *saveptr;
    char *split_line;
    char **entries = malloc(MAX_TASK_ENTRY * sizeof(char *));
    int count = 0;
    split_line = strtok_r(line, " ", &saveptr);

    while (split_line != NULL)
    {
        // printf(" %s\n", split_line);
        // put into array
        entries[count] = malloc(strlen(split_line) + 1);
        strcpy(entries[count], split_line);
        count++;

        split_line = strtok_r(NULL, " ", &saveptr);
    }
    print_array(entries, count);
    return entries;
}

void print_array(char **array, int count)
{
    for (int i = 0; i < count; i++)
    {
        printf("entry %d: %s\n", i, array[i]);
    }
}
