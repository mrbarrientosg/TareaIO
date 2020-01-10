#ifndef alloc_h
#define alloc_h

#include <stdlib.h>

int
checked_alloc_n (void *ptr, size_t size, size_t count);

int
checked_realloc_n (void *ptr, size_t size, size_t count);

# define ALLOC(ptr) \
checked_alloc_n (&(ptr), sizeof (*(ptr)), 1)

# define ALLOC_N(ptr, count) \
checked_alloc_n (&(ptr), sizeof (*(ptr)), count)

# define REALLOC_N(ptr, count) \
checked_realloc_n (&(ptr), sizeof (*(ptr)), count)

# define FREE(ptr) \
  do { \
    if (ptr == NULL)\
        break; \
    free(ptr); \
    (ptr) = NULL; \
  } \
  while (0)

#endif /* alloc_h */
