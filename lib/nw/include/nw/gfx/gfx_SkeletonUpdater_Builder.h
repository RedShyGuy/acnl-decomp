#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SkeletonUpdater.h"

// (the class name is from the reference symbols)
class nw::gfx::SkeletonUpdater::Builder
{
public:
    // NULL: no memory
    SkeletonUpdater* Create(nw::os::IAllocator* pAllocator); // 0x0049BEFC | nintendogs:callgraph [tier A]
};
