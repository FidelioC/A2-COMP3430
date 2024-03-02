#include <sys/queue.h>

#include "tasks.c"

typedef struct
{
    Task *task;
    STAILQ_ENTRY(queue_four)
    pointers;
} queue_four;
