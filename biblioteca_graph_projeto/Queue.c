#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

struct Queue {
    int size;
    int begin, end;
    void **elements;
};

Queue *Queue_alloc(int size) {
    Queue *q = malloc(sizeof(Queue));

    if (q) {
        q->size = size;
        q->begin = q->end = -1;
        q->elements = calloc(size, sizeof(void*));

        if (q->elements == NULL) {
            free(q);
            q = NULL;
        }
    }

    return q;
}

void Queue_free(Queue *q) {
    if (q) {
        free(q->elements);
        free(q);
    }
}

bool Queue_isEmpty(Queue *q) {
    if (q) {
        return q->begin == -1;
    }

    return true;
}

int Queue_next(Queue *q, int index) {
    if (q && index >= 0) {
        return (index + 1) % q->size;
    }

    return 0;
}

bool Queue_isFull(Queue *q) {
    if (q) {
        return Queue_next(q, q->end) == q->begin;
    }

    return false;
}

void Queue_push(Queue *q, void *item) {
    if (q && !Queue_isFull(q)) {
        if (Queue_isEmpty(q))
            q->end = q->begin = 0;        
        else
            q->end = Queue_next(q, q->end);

        q->elements[q->end] = item;
    }
}

void *Queue_pop(Queue *q) {
    void *begin = NULL;
    
    if (q && !Queue_isEmpty(q)) {
        begin = q->elements[q->begin];

        if (q->begin == q->end)
            q->begin = q->end = -1;
        else
            q->begin = Queue_next(q, q->begin);
    }

    return begin;
}

void *Queue_begin(Queue *q) {
    if (q && !Queue_isEmpty(q)) {
        return q->elements[q->begin];
    }

    return NULL;
}

void *Queue_end(Queue *q) {
    if (q && !Queue_isEmpty(q)) {
        return q->elements[q->end];
    }

    return NULL;
}

void Queue_print(Queue *q, void (*print)(void*)) {
    if (q && !Queue_isEmpty(q)) {
        for (int i = q->begin; i != q->end; i = Queue_next(q, i)) {
            print(q->elements[i]);
        }
        if (q->begin != q->end)
            print(q->elements[q->end]);
        printf("\n");
    }
}