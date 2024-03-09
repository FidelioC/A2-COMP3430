#include "queue.h"

void enqueue(Node *item, Node **head, Node **tail)
{
    if (*head == NULL)
    {
        *head = *tail = item;
    }
    else
    {
        item->next = NULL;
        (*tail)->next = item;
        *tail = item;
    }
}

Node *dequeue(Node **head, Node **tail)
{
    Node *result;
    if (*head == NULL)
    {
        return NULL;
    }
    result = *head;
    *head = (*head)->next;
    if (*head == NULL)
    {
        *tail = NULL;
    }
    return result;
}

void print_queue(Node *head, char *queue_name)
{
    printf("PRINTING QUEUE: %s\n", queue_name);
    Node *curr = head;
    while (curr != NULL)
    {
        // printf("Task: %s\n", curr->task->task_name);
        print_task(curr->task);
        curr = curr->next;
    }
}

bool is_queue_empty(Node **head)
{
    return *head == NULL;
}
