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
void make_delay(int delay_time);
void create_worker_threads(int total_workers, pthread_t *worker_threads);

typedef struct
{
    FILE *file;
    bool init_read;
    long file_position;
} ReadFileParams;

int main(void)
{
    int total_workers = 3; // constant will change later
    ReadFileParams args;
    pthread_t scheduler_thread;
    pthread_t reader_thread;
    pthread_t dispatcher_thread;
    pthread_t worker_threads[total_workers];

    // init param
    args.file = fopen("tasks.txt", "r");
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
    pthread_create(&scheduler_thread, NULL, scheduler_thread_funct, NULL);
    pthread_create(&dispatcher_thread, NULL, dispatcher, NULL);
    create_worker_threads(total_workers, worker_threads);

    // wait for threads
    pthread_join(reader_thread, NULL);
    pthread_join(scheduler_thread, NULL);
    pthread_join(dispatcher_thread, NULL);
    for (int i = 0; i < total_workers; i++)
    {
        pthread_join(worker_threads[i], NULL);
    }

    // print_queue(queue_four_head, "queue_four");
    fclose(args.file);

    return EXIT_SUCCESS;
}

void create_worker_threads(int total_workers, pthread_t *worker_threads)
{
    // void *worker_id = malloc(sizeof(int));

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

    // init cond
    pthread_cond_init(&dispatcher_cond, NULL);
    pthread_cond_init(&worker_cond, NULL);
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
        odds = atof(splitted_array[3]);
        Task *new_task = create_new_task(name, type, length, odds, global_time);
        Node *new_node = (Node *)malloc(sizeof(Node));
        new_node->task = new_task;

        // Rule 3: when job enters the system, placed at the highest priority
        pthread_mutex_lock(&queue_four_lock);
        enqueue(new_node, &queue_four_head, &queue_four_tail);
        pthread_mutex_unlock(&queue_four_lock);
    }
    else
    {
        make_delay(type);
    }
}

void make_delay(int delay_time)
{
    struct timespec start_time;
    struct timespec current_time;
    long elapsed_time_seconds;
    long elapsed_time_nanoseconds;
    long total_elapsed_time;
    printf("DELAYING FOR %d microseconds\n", delay_time);
    clock_gettime(CLOCK_REALTIME, &start_time);
    // loop for specified delayed time
    while (1)
    {
        clock_gettime(CLOCK_REALTIME, &current_time);

        elapsed_time_seconds = (current_time.tv_sec - start_time.tv_sec) * 1000000;
        elapsed_time_nanoseconds = (current_time.tv_nsec - start_time.tv_nsec) / 1000;
        total_elapsed_time = elapsed_time_seconds + elapsed_time_nanoseconds;
        // printf("Total elapsed time: %ld\n", total_elapsed_time);
        if (total_elapsed_time >= delay_time)
        {
            break;
        }
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
            make_delay(atoi(splitted_array[1]));
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
