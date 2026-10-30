// The errno accessors of the ARM C library (Thumb code in the original); the variable is in clib_Errno.cpp.

#include "decomp.h"
#include "clib/clib_Thumb.h"

extern "C" {

int* __aeabi_errno_addr();

// 0x00101A04 (name is ours)
int __read_errno()
{
    return *__aeabi_errno_addr();
}

// 0x00101A0E (name is ours)
void __set_errno(int value)
{
    *__aeabi_errno_addr() = value;
}

} // extern "C"
