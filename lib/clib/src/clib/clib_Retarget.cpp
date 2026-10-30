// The file operations below the streams of the C library (the functions a program supplies to the library): the
// program has no files, every one of them fails with -1. All names are ours.

#include "decomp.h"
#include "types.h"

extern "C" {

// (whether the handle is an interactive device)
// 0x002F99C0 (name is ours)
int __stdio_is_terminal(int handle)
{
    return -1;
}

// 0x002F99C8 (name is ours)
int __stdio_file_length(int handle)
{
    return -1;
}

// (the handle of the file, -1)
// 0x007A10D0 (name is ours)
int __stdio_open(const char* pName, u32 mode)
{
    return -1;
}

// 0x007A10C8 (name is ours)
int __stdio_close(int handle)
{
    return -1;
}

// 0x007A10D8 (name is ours)
int __stdio_seek(int handle, s32 position)
{
    return -1;
}

// (the bytes not written)
// 0x007A10E0 (name is ours)
int __stdio_write_file(int handle, const u8* pData, int size, u32 flags)
{
    return -1;
}

// (a signal of the C library: the program ignores it)
// 0x002F9C9C (name is ours)
void __raise_signal(int signal, const char* pMessage)
{
    // empty (in the original too)
}

} // extern "C"
