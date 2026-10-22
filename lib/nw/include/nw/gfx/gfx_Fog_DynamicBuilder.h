#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_Fog.h"

// Builds a fog with a resource of its own (the class name and Create are from the reference symbols,
// the member is ours).
class nw::gfx::Fog::DynamicBuilder
{
public:
    // the fog (NULL: no memory)
    Fog* Create(os::IAllocator* pAllocator); // 0x004A4BE4 | nintendogs:bytes [tier A]

    Description m_Description; // 0x0
};
