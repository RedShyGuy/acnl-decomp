#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex10RootObjectE @ 0x008CDFC8
class RootObject
{
public:
    RootObject() {} // (inline, empty)

    // (nex allocates through its MemoryManager)
    static void* operator new(unsigned int size); // 0x003551B4 | fefates:callgraph [tier C]
    static void operator delete(void* p); // 0x00355190 | fefates:callgraph [tier C]
};
} // namespace nex
} // namespace nn
