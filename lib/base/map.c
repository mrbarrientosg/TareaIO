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

#include "../../include/base/map.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <assert.h>

/*
 http://planetmath.org/goodhashtableprimes
 */
static const long primes[] = {
    3, 7, 13, 23, 41, 71, 127, 191, 251, 383, 631, 1087, 1723,
    2803, 4523, 7351, 11959, 19447, 31231, 50683, 81919, 132607,
    214519, 346607, 561109, 907759, 1468927, 2376191, 3845119,
    6221311, 10066421, 16287743, 26354171, 42641881, 68996069,
    111638519, 180634607, 292272623, 472907251
};

static const long primeLength = sizeof(primes) / sizeof(primes[0]);

static const double maxLoadFactor = 0.77;

struct map {
    long size; // tamaño del arreglo
    long count; // cantidad actual
    long current;
    int primeIndex: 6; // indice del primo en cual esta.
    long loadFactor; // el factor de carga que tiene el mapa.
    
    void const **buckets;
    
    map_hash_callback hash;
    map_equal_callback equal;
    map_release_object_callback release;
};

static long long
djb2_hash(const void *key) {
    long long hash = 5381;
    
    const char *ptr = key;
    
    while (*ptr) {
        hash = ((hash << 5) + hash) + tolower(*ptr);
        ptr++;
    }
    
    return hash;
}

static int
equal_string(const void *key1, const void *key2) {
    const char *A = key1;
    const char *B = key2;
    
    return strcmp(A, B) == 0;
}

const map_hash_callback kStringMapHashCallBack = djb2_hash;

const map_equal_callback kStringMapEqualCallBack = equal_string;

static void
map_enlarge(map *map) {
    if (map->primeIndex == (primeLength - 1)) return; // se verifica si super el ultimo indice.
    
    void const ** oldBucket = map->buckets;
    
    if (oldBucket == NULL) return;
    
    long newSize = primes[++(map->primeIndex)]; // se obtiene el nuevo size y se aumento el primeIndex.
    long oldSize = primes[(map->primeIndex - 1)];
    
    map->buckets = (void const **) malloc(sizeof (void const *) * newSize);
    memset (map->buckets, 0, newSize * sizeof (void const *));
    map->count = 0;
    map->size = newSize;
    map->loadFactor = (long) ceil (newSize * maxLoadFactor);
    
    long i;
    
    for (i = 0; i < oldSize; i++) {
        if (oldBucket[i] != NULL) {
            if (oldBucket[i] != NULL) {
                map_insert (map, oldBucket[i]);
            } else {
                free ((void *)oldBucket[i]);
            }
        }
    }
    
    free (oldBucket);
}

static long
map_quadratic_probing(map *map, const void *value) {
    assert (map->hash != NULL && map->equal != NULL);
    
    long long hash = llabs (map->hash(value));
    long idx = hash % map->size;
    long i = 0;
    
    while (map->buckets[idx] != NULL && map->equal (map->buckets[idx] , value) == 0) {
        idx = (idx + i * i) % map->size;
        i += 1;
//        idx = (idx + 1) % map->size;
    }
    
    return idx;
}

map *
map_init (map_hash_callback hash, map_equal_callback equal, map_release_object_callback release) {
    map *new = (map *) malloc (sizeof (map));
    
    if (new == NULL)
        return NULL;
        
    new->primeIndex = 0; // se guarda el indice del actual numero primo.
    new->count = 0;
    new->current = 0;
    new->loadFactor = (long) ceil (primes[0] * maxLoadFactor); // se calcula el factor de carga.
    new->size = primes[0];
    new->buckets = (void const **) malloc (sizeof (void const *) * primes[0]);
    memset (new->buckets, 0, primes[0] * sizeof (void const *)); // se inicializa en null.
    new->hash = hash;
    new->equal = equal;
    new->release = release;
    
    return new;
}

long
map_size (map *map) {
    assert (map != NULL);
    
    if (map->buckets == NULL)
        return 0;
    
    return map->count;
}

int
map_empty (map *map) {
    assert (map != NULL);
    
    return map->count == 0;
}

void
map_insert (map *map, const void * value) {
    assert (map != NULL); // El mapa no puede ser NULL.
    
    long idx = map_quadratic_probing (map, value);
    
    if (map->buckets[idx] == NULL) {
        map->buckets[idx] = value;
        map->count += 1;
    }
    
    if ((map->count) >= map->loadFactor) map_enlarge (map); // si la cantidad supera al factor de carga se hace el enlarge del mapa.
}

void *
map_remove_key (map *map, const void *value) {
    assert (map != NULL); // El mapa no puede ser NULL.
    
    long idx = map_quadratic_probing (map, value);
    
    if (map->buckets[idx] == NULL)
        return NULL;
    
    void * aux = NULL;
    
    if (map->buckets[idx] != NULL) {
        aux = (void *) map->buckets[idx];
        
        if (map->release != NULL) {
            map->release (aux);
            aux = NULL;
        }
    }
    
    free ((void *)map->buckets[idx]);
    
    map->buckets[idx] = NULL;
   
    map->count -= 1;
    
    return aux;
}

void *
map_search_key (map *map, const void *value) {
    assert (map != NULL); // El mapa no puede ser NULL.

    long idx = map_quadratic_probing (map, value);
        
    if (map->buckets[idx] == NULL)
        return NULL;
    
    map->current = idx;
    
    return (void * )map->buckets[idx];
}

void *
map_first (map *map) {
    assert (map != NULL); // El mapa no puede ser NULL.

    if (map->buckets == NULL)
        return NULL;
    
    long i;
    
    for (i = 0; i < map->size; i++) {
        if (map->buckets[i] != NULL) {
            map->current = i;
            return (void *)map->buckets[i];
        }
    }
    
    return NULL;
}

void *
map_next (map *map) {
    assert (map != NULL); // El mapa no puede ser NULL.

    if (map->buckets == NULL)
        return NULL;
    
    long i;
    
    for (i = (map->current + 1); i < map->size; i++) {
        if (map->buckets[i] != NULL) {
            map->current = i;
            return (void *)map->buckets[i];
        }
    }
    
    return NULL;
}

void
map_release (map **map) {
    assert (map != NULL); // El mapa no puede ser NULL.
    assert (*map != NULL);
    
    long i;
    
    for (i = 0; i < (*map)->size; i++) {
        if ((*map)->buckets[i] != NULL) {
            free ((void *)(*map)->buckets[i]);
        }
    }
    
    if ((*map)->buckets != NULL)
        free ((*map)->buckets);
    
    free (*map);
    *map = NULL;
}
