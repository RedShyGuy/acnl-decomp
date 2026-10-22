#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_HemiSphereLight.h"

// Builds a light with a resource of its own (the class name and Create are from the reference
// symbols, the member is ours).
class nw::gfx::HemiSphereLight::DynamicBuilder
{
public:
    // the light (NULL: no memory)
    HemiSphereLight* Create(os::IAllocator* pAllocator); // 0x0049A9E0 | nintendogs:bytes [tier A]

    Description m_Description; // 0x0
};
