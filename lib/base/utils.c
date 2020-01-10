#define INLINE
#include "../../include/base/utils.h"
#include "../../include/base/alloc.h"
#include "../../include/base/debug_log.h"
#include <string.h>
#include <stdarg.h>

long int seed = 12345678;

double
rnorm (long *idum)
{
    long k;
    double ans;

    k = (*idum)/IQ;
    *idum = IA * (*idum - k * IQ) - IR * k;
    if (*idum < 0) *idum += IM;
    ans = AM * (*idum);

    return ans;
}

long int
random_number (long *idum)
{
    long k;

    k = (*idum)/IQ;
    *idum = IA * (*idum - k * IQ) - IR * k;
    if (*idum < 0) *idum += IM;

    return *idum;
}

char *
read_line(FILE * fp) {
    int ch;
    int i = 0;
    size_t buff_len = 0;
    char *buffer = NULL;

    if (ALLOC_N(buffer, buff_len + 1) == -1)
        FATAL_ERROR("No se puede asignar memoria.");

    while ((ch = fgetc(fp)) != '\n' && ch != EOF)
    {
        buff_len += 1;
        if (REALLOC_N(buffer, buff_len + 1) == -1) {
            FREE(buffer);
            FATAL_ERROR("No se puede asignar memoria.");
        }
        buffer[i] = (char) ch;
        i += 1;
    }
    buffer[i] = '\0';

    if (ch == EOF && (i == 0 || ferror(fp)))
    {
        if (buffer != NULL)
            FREE(buffer);
        return NULL;
    }

    return buffer;
}

char *
strdup(const char *str)
{
    if (str == NULL)
        return NULL;
    
    size_t len;
    char *copy;

    len = strlen(str) + 1;
    if (!(copy = malloc(len)))
        return (NULL);
    memcpy(copy, str, len);
    return (copy);
}

void
printif (FILE *fp, const char *format, ...)
{
    if (fp == NULL)
        return;
    
    va_list arg;
    va_start (arg, format);
    vfprintf(fp, format, arg);
    va_end (arg);
}



