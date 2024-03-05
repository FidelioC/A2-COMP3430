#ifndef QUEUE_H
#define QUEUE_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "tasks.h"

typedef struct _node
{
    Task *task;
    struct _node *next;
} Node;

void enqueue(Node *item, Node **head, Node **tail);
Node *dequeue(Node **head, Node **tail);
void print_queue(Node *head, char *queue_name);
bool is_queue_empty(Node **head);

#endif
