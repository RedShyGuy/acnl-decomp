#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_SceneContext.h"

// The sizes of the arrays of a scene context (the class is named after the other builders; member
// names are ours).
class nw::gfx::SceneContext::Builder
{
public:
    // NULL: no memory
    SceneContext* Create(nw::os::IAllocator* pAllocator); // 0x0048F6A4 (name is ours)

    bool m_IsFixedSizeMemory;   // 0x00 the arrays are allocated with these sizes and do not grow
    s32 m_MaxSceneNodes;        // 0x04
    s32 m_MaxUnknown0x18Nodes;  // 0x08
    s32 m_MaxModels;            // 0x0C
    s32 m_MaxSkeletalModels;    // 0x10
    s32 m_MaxCameras;           // 0x14
    s32 m_MaxLights;            // 0x18
    s32 m_MaxFragmentLights;    // 0x1C
    s32 m_MaxVertexLights;      // 0x20
    s32 m_MaxHemiSphereLights;  // 0x24
    s32 m_MaxAmbientLights;     // 0x28
    s32 m_MaxFogs;              // 0x2C
    s32 m_MaxParticleSets;      // 0x30
    s32 m_MaxParticleEmitters;  // 0x34
    s32 m_MaxParticleModels;    // 0x38
    s32 m_MaxAnimatableNodes;   // 0x3C
};
ASSERT_SIZE(nw::gfx::SceneContext::Builder, 0x40);
