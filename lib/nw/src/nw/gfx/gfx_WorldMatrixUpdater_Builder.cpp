#include "nw/gfx/gfx_WorldMatrixUpdater_Builder.h"

#include <new>

// 0x0049FA6C | nintendogs:callgraph [tier A]
nw::gfx::WorldMatrixUpdater* nw::gfx::WorldMatrixUpdater::Builder::Create(nw::os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(WorldMatrixUpdater), 4);
    return pMemory != nullptr ? new (pMemory) WorldMatrixUpdater(pAllocator) : nullptr;
}
