#pragma once

// string.h of the C library of the program (the ARM C library ARMCC linked in, decompiled in lib/clib). This directory
// comes before the system headers: the decompiled code must not use the C or C++ library of GCC.

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* destination, const void* source, size_t size);
void* memmove(void* destination, const void* source, size_t size);
void* memset(void* destination, int value, size_t size);
int memcmp(const void* first, const void* second, size_t size);
void* memchr(const void* memory, int c, size_t size);
size_t strlen(const char* string);
char* strcpy(char* destination, const char* source);
char* strncpy(char* destination, const char* source, size_t size);
char* strcat(char* destination, const char* source);
char* strncat(char* destination, const char* source, size_t size);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t size);
char* strchr(const char* string, int c);
char* strrchr(const char* string, int c);
char* strstr(const char* string, const char* part);
int strcoll(const char* a, const char* b);
size_t strxfrm(char* pDestination, const char* pSource, size_t size);
// (the BSD function of the library: copies at most size - 1 characters and terminates; the length of source)
size_t strlcpy(char* destination, const char* source, size_t size);

#ifdef __cplusplus
}
#endif
