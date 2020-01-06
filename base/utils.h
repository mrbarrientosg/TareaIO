//
//  utils.h
//  TareaIO
//
//  Created by Matias Barrientos on 02-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef utils_h
#define utils_h

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836

#define XORSWAP(a, b) \
((&(a) == &(b)) ? (a) \
                : ((a) ^= (b), (b) ^= (a), \
                   (a) ^= (b)))

extern long int seed; // Seed para generar numeros randoms

double rnorm (long *idum); // Funcion para generar numeros randoms [0, 1]

long int random_number (long *idum);

char *read_line(FILE * fp);

char *strdup(const char *str);

void *copy (void *ptr, size_t size);

void *copy_n (void *ptr, size_t size, size_t n);

void printif (FILE *fp, const char *, ...);

INLINE int min(int a, int b)
{
    if (a > b)
        return b;
    return a;
}

INLINE int max(int a, int b)
{
    if (a < b)
        return b;
    return a;
}


#endif /* utils_h */
