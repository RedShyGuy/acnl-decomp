#pragma once

// wctype.h of the C library of the program (the ARM C library ARMCC linked in, decompiled in lib/clib). This directory
// comes before the system headers: the decompiled code must not use the C or C++ library of GCC.

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int wint_t;
int iswspace(wint_t c);
wint_t towlower(wint_t c);
wint_t towupper(wint_t c);

#ifdef __cplusplus
}
#endif
