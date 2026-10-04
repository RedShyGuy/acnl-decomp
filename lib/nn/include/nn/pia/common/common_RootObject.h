#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common10RootObjectE @ 0x008CFE08
//
// Base of the pia classes: allocates them on the heap of the module that is set up at the moment
// (HeapManager::GetHeap). It has no members and no virtual functions.
class RootObject
{
public:
    static void* operator new(size_t size); // 0x004266C4 | fefates:callgraph
    static void* operator new[](size_t size); // 0x0053B2DC | fefates:callgraph
    // (the deleting destructors of the pia classes call it; address from them)
    static void operator delete(void* p); // 0x004266B4
    static void operator delete[](void* p); // 0x00426694 | fefates:callgraph
};
} // namespace common
} // namespace pia
} // namespace nn
