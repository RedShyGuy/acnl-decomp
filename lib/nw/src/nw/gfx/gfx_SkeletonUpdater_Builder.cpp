#include "nw/gfx/gfx_SkeletonUpdater_Builder.h"

#include <new>

// 0x0049BEFC | nintendogs:callgraph [tier A]
nw::gfx::SkeletonUpdater* nw::gfx::SkeletonUpdater::Builder::Create(nw::os::IAllocator* pAllocator)
{
    void* pMemory = pAllocator->Alloc(sizeof(SkeletonUpdater), 4);
    return pMemory != nullptr ? new (pMemory) SkeletonUpdater(pAllocator) : nullptr;
}
