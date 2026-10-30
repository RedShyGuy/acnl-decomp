// The C++ run-time support of the ARM C library (the parts in ARM code; the guard functions and the array
// constructors are Thumb code).

#include "decomp.h"

// registers nothing: the program never ends, so no destructor of a static object runs (armlink also replaced
// most calls by nops)
// 0x002FE980 | fefates:callgraph [tier C]
extern "C" void __aeabi_atexit(void* object, void (*destroy)(void*), void* handle)
{
    // empty (in the original too)
}
