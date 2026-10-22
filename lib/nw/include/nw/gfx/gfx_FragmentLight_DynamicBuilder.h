#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_FragmentLight.h"

// Builds a fragment light with a resource of its own (the class name and Create are from the
// reference symbols, the member is ours).
class nw::gfx::FragmentLight::DynamicBuilder
{
public:
    // the light (NULL: no memory)
    FragmentLight* Create(os::IAllocator* pAllocator); // 0x00492AD8 | nintendogs:bytes [tier A]

    Description m_Description; // 0x0
};
