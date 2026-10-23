#include "nw/gfx/gfx_SceneTraverser_Builder.h"

#include <new>

// 0x0049A7F8 | nintendogs:bytes [tier A]
nw::gfx::SceneTraverser* nw::gfx::SceneTraverser::Builder::Create(nw::os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(SceneTraverser), 4);
    return pMemory != nullptr ? new (pMemory) SceneTraverser(pAllocator) : nullptr;
}
