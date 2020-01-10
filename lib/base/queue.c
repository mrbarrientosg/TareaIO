/**
 MIT License
 
 Copyright (c) 2018 Matias Barrientos.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "../../include/base/queue.h"
#include <stdlib.h>
#include <assert.h>

typedef struct node node;

struct node {
    const void *data;
    node *next;
};

struct queue {
    node *tail;
    long count;
    queue_release_object_callback release;
};

static node *
node_init (const void *data) {
    node *new = (node *) malloc (sizeof (node));
   
    assert (new != NULL);
    
    new->data = data;
    new->next = NULL;
   
    return new;
}

queue *
queue_init (queue_release_object_callback release) {
    queue *new = (queue *) malloc (sizeof (queue));
    
    assert (new != NULL);
    
    new->tail = NULL;
    new->count = 0;
    new->release = release;
    
    return new;
}

void
queue_push (queue *queue, const void *data) {
    assert (queue != NULL);
    
    node *new = node_init (data);
    
    if (queue->tail == NULL) {
        new->next = new;
    } else {
        new->next = queue->tail->next;
        queue->tail->next = new;
    }
    
    queue->tail = new;
    queue->count += 1;
}

void *
queue_pop (queue *queue) {
    assert (queue != NULL);
    
    if (queue->tail == NULL)
        return NULL;
    
    node *aux = queue->tail->next;
    
    void *data = (void *) aux->data;
    
    if (queue->release != NULL) {
        queue->release (data);
        data = NULL;
    }
    
    if (aux == queue->tail) {
        queue->tail = NULL;
    } else {
        queue->tail->next = aux->next;
    }
    
    free (aux);
    
    queue->count -= 1;
    
    return data;
}

void *
queue_front (queue *queue) {
    assert (queue != NULL);
    
    if (queue->tail == NULL || queue->tail->next == NULL)
        return NULL;
    
    return (void *) queue->tail->next->data;
}

long
queue_size (queue *queue) {
    assert (queue != NULL);
    
    if (queue->tail == NULL)
        return 0;
    
    return queue->count;
}

int
queue_empty(queue *queue) {
    assert (queue != NULL);
    
    return queue->count == 0;
}

int
queue_contains (queue *queue, const void *data, int (*equals)(const void *a, const void *b)) {
    node *aux = queue->tail;

    if (aux == NULL)
        return 0;

    const void *first = aux;

    do {
        if (equals(aux->data, data))
            return 1;
        aux = aux->next;
    } while (aux != first);

    return 0;
}

void
queue_release (queue **queue) {
    assert (queue != NULL);
    assert ((*queue) != NULL);
    
    while ((*queue)->tail != NULL) {
        queue_pop (*queue);
    }
    
    free (*queue);
    *queue = NULL;
}
