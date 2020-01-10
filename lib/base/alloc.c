#include "alloc.h"
#include <stdlib.h>
#include <stddef.h>

int
checked_alloc_n (void *ptr, size_t size, size_t count) {
    
    if (size == 0 || count == 0) {
        *(void **) ptr = NULL;
        return 0;
    }
    
    *(void **) ptr = calloc (count, size);
    
    if (*(void **) ptr == NULL)
        return -1;
    
    return 0;
}

int
checked_realloc_n (void *ptr, size_t size, size_t count) {
    
    void *temp;
    
    if (size == 0 || count == 0) {
        *(void **) ptr = NULL;
        return 0;
    }
    
    temp = realloc (*(void **) ptr, size * count);
    
    if (temp == NULL)
        return -1;
    
    *(void **) ptr = temp;
    
    return 0;
}
