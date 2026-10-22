#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_AmbientLight.h"

// Builds a light with a resource of its own (the class name and Create are from the reference
// symbols, the member is ours).
class nw::gfx::AmbientLight::DynamicBuilder
{
public:
    // the light (NULL: no memory)
    AmbientLight* Create(os::IAllocator* pAllocator); // 0x0048DB5C | nintendogs:bytes [tier A]

    Description m_Description; // 0x0
};
