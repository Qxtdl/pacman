#pragma once

#include <stddef.h>

void *smalloc(size_t size);
void *srealloc(void *ptr, size_t size);
void *scalloc(size_t nmemb, size_t size);
int clamp(int v, int min, int max);
int random_int(int min, int max);