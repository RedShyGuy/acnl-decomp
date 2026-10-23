#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneTraverser.h"

// (the class name is from the reference symbols)
class nw::gfx::SceneTraverser::Builder
{
public:
    // NULL: no memory
    SceneTraverser* Create(nw::os::IAllocator* pAllocator); // 0x0049A7F8 | nintendogs:bytes [tier A]
};
