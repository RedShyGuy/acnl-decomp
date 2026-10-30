// The C++ run-time support of the ARM C library (the parts in ARM code; the guard functions and the array
// constructors are Thumb code).

#include "decomp.h"
#include "nn/os/os_ThreadLocalRegion.h"

// registers nothing: the program never ends, so no destructor of a static object runs (armlink also replaced
// most calls by nops)
// 0x002FE980 | fefates:callgraph [tier C]
extern "C" void __aeabi_atexit(void* object, void (*destroy)(void*), void* handle)
{
    // empty (in the original too)
}

// the slot of the thread's exception state in the thread local region
// 0x0013A278 (name is ours)
extern "C" void** __cxa_eh_globals_slot()
{
    return reinterpret_cast<void**>(reinterpret_cast<u8*>(nn::os::detail::GetThreadLocalRegion()) + 0x5C);
}
