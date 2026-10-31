#pragma once

// stdlib.h of the C library of the program (the ARM C library ARMCC linked in, decompiled in lib/clib). This directory
// comes before the system headers: the decompiled code must not use the C or C++ library of GCC.

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* malloc(size_t size);
// (declared by the library; the program never calls it)
void* calloc(size_t count, size_t size);
void free(void* memory);
void abort(void);
int atoi(const char* string);
long strtol(const char* string, char** end, int base);
unsigned long strtoul(const char* string, char** end, int base);
unsigned long long strtoull(const char* string, char** end, int base);
long long llabs(long long value);
void qsort(void* base, size_t count, size_t size, int (*compare)(const void*, const void*));
void* bsearch(const void* key, const void* base, size_t count, size_t size, int (*compare)(const void*, const void*));

#ifdef __cplusplus
}
#endif
