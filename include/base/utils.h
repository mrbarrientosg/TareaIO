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

extern long int seed; // Seed para generar numeros random

double rnorm (long *idum); // Funcion para generar numeros random [0, 1]

long int random_number (long *idum); // Funcion para generar random

char *read_line(FILE * fp);

char *strdup(const char *str);

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
