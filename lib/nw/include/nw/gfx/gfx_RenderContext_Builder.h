#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_RenderContext.h"

// The settings of a render context (the class name and Create are from the reference symbols, the
// members are ours).
class nw::gfx::RenderContext::Builder
{
public:
    // a render context (NULL: no memory); a missing particle activator is made first
    RenderContext* Create(os::IAllocator* pAllocator); // 0x00496E6C | nintendogs:callseq [tier A]

    s32 m_MaxCameras;                                // 0x00
    s32 m_MaxLightSets;                              // 0x04
    s32 m_MaxFogs;                                   // 0x08
    s32 m_MaxVertexLights;                           // 0x0C
    IMaterialActivator* m_pParticleMaterialActivator; // 0x10
};
ASSERT_SIZE(nw::gfx::RenderContext::Builder, 0x14);
