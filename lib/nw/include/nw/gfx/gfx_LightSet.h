#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_GfxObject.h"
#include "nw/gfx/res/gfx_ResSceneEnvironmentSetting.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
class AmbientLight;
class HemiSphereLight;
class Light;

// RTTI N2nw3gfx8LightSetE @ 0x008D07FC
// vtable 0x00902A84 (vptr 0x00902A8C), offset_to_top 0, 3 entries
// The lights a scene environment activates together: an ambient and a hemisphere light, the vertex
// lights and up to 8 fragment lights (member names are ours).
class LightSet : public ::nw::gfx::GfxObject
{
public:
    // the room for the vertex lights (names are ours)
    struct Description
    {
        bool isFixedSizeMemory; // 0x0 the array does not grow
        s32 maxVertexLights;    // 0x4
    };
    // the fragment lights of the hardware
    static const int FRAGMENT_LIGHT_MAX = 8;

    // (inline)
    LightSet(os::IAllocator* pAllocator, res::ResLightSet resLightSet)
        : GfxObject(pAllocator), m_ResLightSet(resLightSet), m_pAmbientLight(nullptr), m_pHemiSphereLight(nullptr),
          m_FragmentLights(m_FragmentLightBuffer, FRAGMENT_LIGHT_MAX, nullptr, ut::MoveArray<Light*>::KIND_INLINE)
    {
    }
    virtual ~LightSet();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C87C slot 0x08

    // (symbols.json calls them constructors)
    static LightSet* Create(const Description& description, os::IAllocator* pAllocator); // 0x004AEA3C (name is ours)
    static LightSet* Create(res::ResLightSet resLightSet, os::IAllocator* pAllocator); // 0x004AEC1C (name is ours)

    static const TypeInfo s_TypeInfo;

    res::ResLightSet m_ResLightSet;           // 0x08
    AmbientLight* m_pAmbientLight;            // 0x0C
    HemiSphereLight* m_pHemiSphereLight;      // 0x10
    ut::MoveArray<Light*> m_VertexLights;     // 0x14
    u8 m_Unknown0x24[0x1C];                   // 0x24
    ut::MoveArray<Light*> m_FragmentLights;   // 0x40 in m_FragmentLightBuffer
    u8 m_Unknown0x50[0x10];                   // 0x50
    Light* m_FragmentLightBuffer[FRAGMENT_LIGHT_MAX]; // 0x60
};
ASSERT_OFFSET(LightSet, m_FragmentLights, 0x40);
ASSERT_SIZE(LightSet, 0x80);
} // namespace gfx
} // namespace nw
