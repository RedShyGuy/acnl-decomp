#pragma once

// sys/types.h for zlib (zconf.h includes it for off_t; the C library of the program has no such header).

#include <stddef.h>

typedef long off_t;
