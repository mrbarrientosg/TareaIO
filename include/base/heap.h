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

#ifndef heap_h
#define heap_h

typedef struct heap heap;

/**
 @typedef
 La función compara dos elemento del tipo que definio el usuario.

 @param value1 Primero parametro.
 @param value2 Segundo parametro.
 @return retorna 3 valores con respecto a la comparacion realizada.
    1: p1 es mayor que p2.
    0: p1 es igual a p2.
   -1: p1 es menor que p2.
 */
typedef int (*heap_compare_callback)(const void *value1, const void *value2);

/**
 @typedef
 Prototipo de funcion para eliminar de memoria el objeto que se guarda dentro del heap.
 
 @param object Puntero al objecto guardado.
 */
typedef void (*heap_release_object_callback)(void *object);

/**
 Crea un nuevo puntero de tipo Heap.

 @param compare Función de comparación.
 @param size El tamaño inicial del heap.
 @return Puntero al nuevo Heap creado.
 */
extern heap *heap_init (long size, heap_compare_callback compare, heap_release_object_callback release);

/**
 Inserta un nuevo objecto dentro del heap, hace uso de la funcion comparar para poder insertar.
 Dependiendo de la funcion comparar, va a ser un heap de maximo o de minimo.
 
 Esta función tiene como complejidad:
 
 - Mejor caso: O(1)
 
 - Peor caso: O(log N)

 @param heap Puntero al Heap.
 @param data Puntero a al dato que quiere ser insertado.
 */
extern void heap_push (heap *heap, const void *data);

/**
 Elimina el elemento maximo o minimo del heap.
 Si la cantidad de datos en el Heap es 0, la funcion retorna NULL.
 
 Si la funcion release esta activa, la funcion eliminara
 el dato de la memoria y retornara NULL.
 
 Complejidad: O(log n)

 @param heap Puntero al Heap.
 @return Puntero al dato eliminado del Heap.
 */
extern void *heap_pop (heap *heap);

/**
 Retorna el elemento maximo o minimo del Heap.
 Si la cantidad de datos en el Heap es 0, la funcion retorna NULL.

 Complejidad: O(1)

 @param heap Puntero al Heap.
 @return Puntero al dato eliminado del heap.
 */
extern void *heap_peek (heap *heap);

/**
 Retorna la cantidad de elementos del Heap.
 
 Complejidad: O(1)

 @param heap Puntero al Heap.
 @return Cantidad de elementos del heap.
 */
extern long heap_size (heap *heap);

/**
 Prueba si el Heap está vacía.
 
 Complejidad: O(1)
 
 @param heap Puntero al Heap.
 @return 1 (true) si y solo si el Heap no contiene elementos; 0 (false) lo contrario.
 */
extern int heap_empty (heap *heap);


extern int heap_contains (heap *heap, const void *data, int (*equals)(const void *s1, const void *s2));

/**
 Elimina todo los elementos del Heap, pero no libera memoria de ellos.
 
 Complejidad: O(n)

 @param heap Puntero al Heap.
 */
extern void heap_release (heap **heap);

#endif /* heap_h */
