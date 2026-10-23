#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_WorldMatrixUpdater.h"

// (the class name is from the reference symbols)
class nw::gfx::WorldMatrixUpdater::Builder
{
public:
    // NULL: no memory
    WorldMatrixUpdater* Create(nw::os::IAllocator* pAllocator); // 0x0049FA6C | nintendogs:callgraph [tier A]
};
