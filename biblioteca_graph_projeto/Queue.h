#pragma once

#include <stdbool.h>

typedef struct Queue Queue;

Queue *Queue_alloc(int size);
void   Queue_free(Queue *q);
bool   Queue_isEmpty(Queue *q);
bool   Queue_isFull(Queue *q);
void   Queue_push(Queue *q, void *item);
void  *Queue_pop(Queue *q);
void  *Queue_begin(Queue *q);
void  *Queue_end(Queue *q);
void   Queue_print(Queue *q, void (*print)(void*));