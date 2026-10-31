#pragma once

// stdio.h of the C library of the program (the ARM C library ARMCC linked in, decompiled in lib/clib). This directory
// comes before the system headers: the decompiled code must not use the C or C++ library of GCC.

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
namespace clib {
struct File;
} // namespace clib
typedef clib::File FILE;
#else
typedef struct File FILE;
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern FILE __stdin;
extern FILE __stdout;
extern FILE __stderr;
#define stdin (&__stdin)
#define stdout (&__stdout)
#define stderr (&__stderr)

int printf(const char* pFormat, ...);
int fprintf(FILE* pStream, const char* pFormat, ...);
int sprintf(char* pDestination, const char* pFormat, ...);
int snprintf(char* pDestination, size_t size, const char* pFormat, ...);
int vsprintf(char* pDestination, const char* pFormat, va_list arguments);
int vsnprintf(char* pDestination, size_t size, const char* pFormat, va_list arguments);
int sscanf(const char* pString, const char* pFormat, ...);
FILE* fopen(const char* pName, const char* pMode);
FILE* freopen(const char* pName, const char* pMode, FILE* pStream);
int fclose(FILE* pStream);
int setvbuf(FILE* pStream, char* pBuffer, int mode, size_t size);
long ftell(FILE* pStream);
int fputc(int c, FILE* pStream);
int ferror(FILE* pStream);

#ifdef __cplusplus
}
#endif
