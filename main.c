#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/queue.h>
#include <stdbool.h>

#include "tasks.h"
#include "queue.h"

#define MAX_TASK_ENTRY 4

// global tracking time in microseconds
long global_time = 0;

// Define global variables for the ready queues
Node *queue_four_head = NULL;
Node *queue_four_tail = NULL;

void read_file(FILE *file);
char **split_to_array(char *line);
void print_array(char **array, int count);

// main is the reading thread
int main(void)
{

    FILE *file = fopen("tasks.txt", "r");
    read_file(file);
    print_queue(queue_four_head, "queue_four");
    return EXIT_SUCCESS;
}

void add_task(char **splitted_array)
{
    // TODO: HAVE TO USE LOCK HERE LATER!

    char *name = splitted_array[0];     // task name
    int type = atoi(splitted_array[1]); // task type or delay time
    long length;                        // task length
    float odds;                         // io odds

    if (strcmp(name, "DELAY") != 0)
    {
        printf("adding task: %s\n\n", name);
        length = strtol(splitted_array[2], NULL, 10); // 10 = base 10
        odds = atof(splitted_array[3]);
        Task *new_task = create_new_task(name, type, length, odds, global_time);
        Node *new_node = (Node *)malloc(sizeof(Node));
        new_node->task = new_task;
        enqueue(new_node, &queue_four_head, &queue_four_tail);
    }
}

void read_file(FILE *file)
{
    char *line = NULL;
    char **splitted_array;
    size_t length = 0;
    while (getline(&line, &length, file) != -1)
    {
        printf("line is: %s", line);
        splitted_array = split_to_array(line);
        add_task(splitted_array);
    }
    fclose(file);
    free(line);
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
