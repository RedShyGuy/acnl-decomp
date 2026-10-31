#pragma once

// wchar.h of the C library of the program (the ARM C library ARMCC linked in, decompiled in lib/clib). This directory
// comes before the system headers: the decompiled code must not use the C or C++ library of GCC.

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// the conversion state of the multibyte functions
typedef struct mbstate_t
{
    unsigned int m_State;
    unsigned int _unknown4;
} mbstate_t;

size_t wcslen(const wchar_t* string);
wchar_t* wcscpy(wchar_t* destination, const wchar_t* source);
wchar_t* wcsncpy(wchar_t* destination, const wchar_t* source, size_t size);
wchar_t* wcscat(wchar_t* destination, const wchar_t* source);
int wcscmp(const wchar_t* a, const wchar_t* b);
int wcsncmp(const wchar_t* a, const wchar_t* b, size_t size);
wchar_t* wcsstr(const wchar_t* string, const wchar_t* part);
wchar_t* wcstok(wchar_t* string, const wchar_t* delimiters, wchar_t** next);
wchar_t* wmemcpy(wchar_t* destination, const wchar_t* source, size_t size);
wchar_t* wmemmove(wchar_t* destination, const wchar_t* source, size_t size);
wchar_t* wmemset(wchar_t* destination, wchar_t c, size_t size);
int wcsicmp(const wchar_t* a, const wchar_t* b);
unsigned long wcstoul(const wchar_t* string, wchar_t** end, int base);
size_t wcstombs(char* pDestination, const wchar_t* pSource, size_t size);
int mbsinit(const mbstate_t* state);
int mbrlen(const char* string, size_t size, mbstate_t* state);
int mbrtowc(wchar_t* pDestination, const char* pSource, size_t size, mbstate_t* state);
size_t wcrtomb(char* pDestination, wchar_t c, mbstate_t* state);
int swprintf(wchar_t* pDestination, size_t size, const wchar_t* pFormat, ...);
int vswprintf(wchar_t* pDestination, size_t size, const wchar_t* pFormat, va_list arguments);

#ifdef __cplusplus
}
#endif
