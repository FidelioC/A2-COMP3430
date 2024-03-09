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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "queue.h"
#include "tasks.h"
#include "scheduler.h"
#include "workers.h"
#include "globals.h"
#include "dispatcher.h"

Node *create_node(char *name, int type, long length, int odds);

int main(void)
{
    // Initialize queues
    Node *queue_one_head = NULL, *queue_one_tail = NULL;
    Node *queue_two_head = NULL, *queue_two_tail = NULL;
    Node *queue_three_head = NULL, *queue_three_tail = NULL;
    Node *queue_four_head = NULL, *queue_four_tail = NULL;

    Node *node1 = create_node("Task1", 0, 1000, 10);
    Node *node2 = create_node("Task2", 1, 2000, 20);
    Node *node3 = create_node("Task3", 2, 3000, 30);
    Node *node4 = create_node("Task4", 0, 1500, 15);
    Node *node5 = create_node("Task5", 1, 2500, 25);
    Node *node6 = create_node("Task6", 0, 1200, 12);
    Node *node7 = create_node("Task7", 1, 2200, 22);
    Node *node8 = create_node("Task8", 2, 3200, 32);
    Node *node9 = create_node("Task9", 3, 5000, 50);
    Node *node10 = create_node("Task10", 3, 6000, 60);

    // Enqueue additional nodes into the respective queues
    enqueue(node1, &queue_one_head, &queue_one_tail);
    enqueue(node2, &queue_one_head, &queue_one_tail);
    enqueue(node3, &queue_one_head, &queue_one_tail);

    enqueue(node4, &queue_two_head, &queue_two_tail);
    enqueue(node5, &queue_two_head, &queue_two_tail);

    enqueue(node6, &queue_three_head, &queue_three_tail);
    enqueue(node7, &queue_three_head, &queue_three_tail);
    enqueue(node8, &queue_three_head, &queue_three_tail);

    enqueue(node9, &queue_four_head, &queue_four_tail);
    enqueue(node10, &queue_four_head, &queue_four_tail);

    // Print the contents of each queue after enqueueing the additional tasks
    printf("Queue 1:\n");
    print_queue(queue_one_head, "queue 1");

    printf("Queue 2:\n");
    print_queue(queue_two_head, "queue 2");

    printf("Queue 3:\n");
    print_queue(queue_three_head, "queue 3");

    printf("Queue 4:\n");
    print_queue(queue_four_head, "queue 4");

    move_queues_to_queue_four(&queue_one_head, &queue_one_tail,
                              &queue_two_head, &queue_two_tail,
                              &queue_three_head, &queue_three_tail,
                              &queue_four_head, &queue_four_tail);

    printf("Queue 4:\n");
    print_queue(queue_four_head, "queue 4");
    // You can continue testing or implementing other parts of your program here

    time_spec time1, time2;
    int temp = 0;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
    for (int i = 0; i < 242000000; i++)
        temp += temp;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
    long elapsed = time_spec_to_micro(diff(time1, time2));
    printf("diff is: %ld:%ld\n", diff(time1, time2).tv_sec, diff(time1, time2).tv_nsec);
    printf("elapsed id: %ld\n", elapsed);

    return 0;
}

Node *create_node(char *name, int type, long length, int odds)
{
    Task *new_task = create_new_task(name, type, length, odds, 0);
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->task = new_task;
    new_node->next = NULL;
    return new_node;
}
