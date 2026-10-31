#pragma once

// ctype.h of the C library of the program (the ARM C library ARMCC linked in, decompiled in lib/clib). This directory
// comes before the system headers: the decompiled code must not use the C or C++ library of GCC.

// The classes of the characters come from the table of the locale (__rt_ctype_table); the class tests are inline
// like the macros of the library, tolower/toupper are functions.

#ifdef __cplusplus
extern "C" {
#endif

const unsigned char** __rt_ctype_table(void);
int tolower(int c);
int toupper(int c);

#define __CTYPE_CLASSES(c) ((*__rt_ctype_table())[c])
static inline int isspace(int c) { return __CTYPE_CLASSES(c) & 0x01; }
static inline int ispunct(int c) { return __CTYPE_CLASSES(c) & 0x02; }
static inline int islower(int c) { return __CTYPE_CLASSES(c) & 0x08; }
static inline int isupper(int c) { return __CTYPE_CLASSES(c) & 0x10; }
static inline int isdigit(int c) { return __CTYPE_CLASSES(c) & 0x20; }
static inline int iscntrl(int c) { return __CTYPE_CLASSES(c) & 0x40; }
static inline int isalpha(int c) { return __CTYPE_CLASSES(c) & 0x18; }
static inline int isalnum(int c) { return __CTYPE_CLASSES(c) & 0x38; }
static inline int isxdigit(int c) { return __CTYPE_CLASSES(c) & 0xA0; }

#ifdef __cplusplus
}
#endif
