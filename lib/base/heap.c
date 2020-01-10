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


#include "heap.h"
#include <stdlib.h>
#include <assert.h>

#define PARENT(i) (i - 1) / 2
#define LEFT(i) 2 * i + 1
#define RIGHT(i) 2 * i + 2

struct heap {
    /*! Arreglo de punteros de datos tipo void */
    const void **heapArray;
    
    /*! Cantidad de elementos no nulos */
    long count;
    
    /*! Tamaño del arreglo */
    long size;
    
    /*! Funcion comparar */
    heap_compare_callback compare;
    
    heap_release_object_callback release;
};

/**
 Encargada de ordenar el arreglo del heap con respecto a la comparacion.

 @param heap Puntero a la struct Heap.
 @param i Posicion en que parte el ordenamiento del arreglo.
 */
static void
heapify (heap *heap, long i) {
    long smallest = i;
    
    if (LEFT(i) < heap->count && heap->compare (heap->heapArray[LEFT(i)], heap->heapArray[i]) < 0) {
        smallest = LEFT(i);
    }
    
    if (RIGHT(i) < heap->count && heap->compare (heap->heapArray[RIGHT(i)], heap->heapArray[smallest]) < 0) {
        smallest = RIGHT(i);
    }
    
    if (smallest != i) {
        const void * tmp = heap->heapArray[i];
        heap->heapArray[i] = heap->heapArray[smallest];
        heap->heapArray[smallest] = tmp;
        heapify(heap, smallest);
    }
}

static void
heap_growth (heap *heap) {
    if (heap->count != heap->size)
        return;
    
    const void **new_heap;
    heap->size = heap->size * 2 + 1;
    new_heap = (const void **) realloc (heap->heapArray, heap->size * sizeof (const void *));
    
    assert (new_heap != NULL);
    
    heap->heapArray = new_heap;
}

static long
heap_find_index (heap *heap, const void *data) {
    long i = heap->count;
    
    while (i > 0 && heap->compare (data, heap->heapArray[PARENT(i)]) < 0) {
        heap->heapArray[i] = heap->heapArray[PARENT(i)];
        i = PARENT(i);
    }
    
    return i;
}

heap *
heap_init (long size, heap_compare_callback compare, heap_release_object_callback release) {
    heap * new = (heap *) malloc (sizeof (heap));
    
    assert (new != NULL); // Se verifica que no sea null.
    
    new->heapArray = (const void **) malloc(sizeof (const void *) * size);
    new->size = size;
    new->count = 0;
    new->compare = compare;
    new->release = release;
    
    return new;
}

void
heap_push (heap *heap, const void *data) {
    assert (heap != NULL); // El heap no puede ser null.
    assert (heap->heapArray != NULL); // El heapArray no puede ser null.
    
    heap_growth (heap);
    
    long idx = heap_find_index (heap, data);
    
    heap->heapArray[idx] = data;
    heap->count += 1;
}

void *
heap_pop (heap *heap) {
    assert (heap != NULL); // El heap no puede ser null.
    assert (heap->heapArray != NULL); // El heapArray no puede ser null.
    
    if (heap->count == 0)
        return NULL;
    
    void * data = (void *) heap->heapArray[0];
    
    if (heap->release != NULL) {
        heap->release (data);
        data = NULL;
    }
    
    heap->count -= 1;
    
    heap->heapArray[0] = heap->heapArray[heap->count];
   
    heapify(heap, 0);
    
    return data;
}

void *
heap_peek (heap *heap) {
    assert (heap != NULL); // El heap no puede ser null.
    assert (heap->heapArray != NULL); // El heapArray no puede ser null.
    
    if (heap->count == 0)
        return NULL;
    
    return (void *) heap->heapArray[0];
}

long
heap_size (heap *heap) {
    assert (heap != NULL); // El heap no puede ser null.
    
    if (heap->heapArray == NULL)
        return 0;
    
    return heap->count;
}

int heap_contains (heap *heap, const void *data, int (*equals)(const void *s1, const void *s2)) {
    for (int i = 0; i < heap->count; i++) {
        if (equals(data, heap->heapArray[i]))
            return 1;
    }

    return 0;
}

void
heap_release (heap **heap) {
    assert (heap != NULL); // El heap no puede ser null.
    assert ((*heap) != NULL);
    
    if ((*heap)->heapArray != NULL)
        free ((*heap)->heapArray);
    
    (*heap)->heapArray = NULL;
    (*heap)->count = 0;
    free (*heap);
    *heap = NULL;
}
