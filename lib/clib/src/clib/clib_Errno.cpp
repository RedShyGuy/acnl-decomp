// errno of the ARM C library: one variable for the whole program; the math functions set it through
// __set_errno (EDOM 1, ERANGE 2), which is Thumb code (clib_ErrnoThumb.cpp).

#include "decomp.h"
#include "types.h"

namespace {
// 0x00AF87E8
int s_Errno;
} // namespace

extern "C" {

// 0x002F7A48 (name is ours, after the ARM run-time ABI)
int* __aeabi_errno_addr()
{
    return &s_Errno;
}

} // extern "C"
