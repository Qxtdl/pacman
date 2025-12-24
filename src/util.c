#include <stdlib.h>
#include <stddef.h>

#include "global.h"

void *smalloc(size_t size) {
    void *alloc = malloc(size);
    if (!alloc)
        app_abort("smalloc()", "Failed to allocate memory")
    return alloc;
}

void *srealloc(void *ptr, size_t size) {
    void *alloc = realloc(ptr, size);
    if (!alloc)
        app_abort("srealloc()", "Failed to allocate memory")
    return alloc;    
}