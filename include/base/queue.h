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

#ifndef queue_h
#define queue_h

typedef struct queue queue;

/**
 @typedef
 Prototipo de funcion para eliminar de memoria el objeto que se guarda dentro de la cola.

 @param object Puntero al objecto guardado.
 */
typedef void (*queue_release_object_callback)(void *object);

/**
 Crea un nuevo puntero de tipo Queue.

 @return Puntero a la nueva Queue.
 */
extern queue *queue_init (queue_release_object_callback release);

/**
 Inserta un elemento en la Queue.

 Complejidad: O(1)

 @param queue Puntero a la Queue.
 @param data Puntero al elemento que quiere ser insertado.
 */
extern void queue_push (queue *queue, const void *data);

/**
 Elimina el primer elemento de la cola, si la funcion
 release esta activa, esta eliminara de la memoria
 el puntero al dato guardado y retornara NULL,
 en caso contrario retornara el puntero al dato.

 Complejidad: O(1)

 @param queue Puntero a la Queue.
 @return Puntero al elemento eliminado de la Queue.
 */
extern void *queue_pop (queue *queue);

/**
 Retorna, pero no elimina, la cabeza de la cola, o devuelve nulo si esta cola está vacía.

 Complejidad: O(1)

 @param queue Puntero a la Queue.
 @return Puntero al elemento obtenido de la cabeza de la cola.
 */
extern void *queue_front (queue *queue);

/**
 Retorna la cantidad de elementos en la Queue.

 Complejidad: O(1)

 @param queue Puntero a la Queue.
 @return La cantidad de elementos en la Queue.
 */
extern long queue_size (queue *queue);

/**
 Prueba si la Queue está vacía.

 Complejidad: O(1)

 @param queue Puntero a la Queue.
 @return 1 (true) si y solo si la Queue no contiene elementos; 0 (false) lo contrario.
 */
extern int queue_empty (queue *queue);

extern int queue_contains (queue *queue, const void *data, int (*equals)(const void *a, const void *b));

/**
 Elimina todo los elementos de la cola, si se encuetra la funcion release
 activa, eliminara los datos guardados de la memoria.

 Complejidad: O(n)

 @param queue Puntero a la Queue.
 */
extern void queue_release (queue **queue);

#endif /* queue_h */

