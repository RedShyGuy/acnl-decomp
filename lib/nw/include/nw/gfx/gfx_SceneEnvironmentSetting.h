#pragma once

#include "decomp.h"
#include "nw/gfx/gfx_LightSet.h"
#include "nw/gfx/gfx_SceneObject.h"
#include "nw/gfx/res/gfx_ResSceneEnvironmentSetting.h"
#include "nw/os/os_MemorySizeCalculator.h"
#include "nw/ut/ut_MoveArray.h"

namespace nw {
namespace gfx {
// RTTI N2nw3gfx23SceneEnvironmentSettingE @ 0x008D0748
// vtable 0x00902860 (vptr 0x00902868), offset_to_top 0, 3 entries
// The light sets, cameras and fogs of a scene environment resource, before they are bound to the
// scene (member names are ours).
class SceneEnvironmentSetting : public ::nw::gfx::SceneObject
{
public:
    // (the contents are not known; name from the reference symbols)
    struct Description
    {
        u32 m_Unknown0x0; // 0x0
    };
    // a light set of the setting, with the index it gets in the environment (name is ours)
    struct LightSetSlot
    {
        // (inline)
        LightSetSlot() : index(-1) {}

        s32 index;                  // 0x0 (-1: none yet)
        GfxPtr<LightSet> lightSet;  // 0x4
    };
    // a camera or fog of the setting (the contents are not known; name is ours)
    struct ReferenceSlot
    {
        // (inline) empty
        ~ReferenceSlot() {}

        u32 m_Unknown0x0; // 0x0
        u32 m_Unknown0x4; // 0x4
    };

    // (inline)
    SceneEnvironmentSetting(os::IAllocator* pAllocator, res::ResSceneObject resObject) : SceneObject(pAllocator, resObject) {}
    virtual ~SceneEnvironmentSetting();

    virtual const TypeInfo* GetRuntimeTypeInfo() const { return &s_TypeInfo; } // 0x0073C498 slot 0x08

    // the arrays of the resource; the light sets made (name is ours)
    void Setup(os::IAllocator* pAllocator, res::ResSceneEnvironmentSetting resSetting); // 0x004A21BC (name is ours)
    static void GetMemorySizeInternal(os::MemorySizeCalculator* pSize, res::ResSceneEnvironmentSetting resSetting, Description description); // 0x004A208C | nintendogs:bytes [tier A]
    // (symbols.json calls it a constructor)
    static SceneEnvironmentSetting* Create(res::ResSceneObject resObject, const Description& description, os::IAllocator* pAllocator); // 0x004A2888 (name is ours)

    static const TypeInfo s_TypeInfo;

    ut::MoveArray<LightSetSlot> m_LightSets;  // 0x0C
    ut::MoveArray<ReferenceSlot> m_Cameras;   // 0x1C
    ut::MoveArray<ReferenceSlot> m_Fogs;      // 0x2C
};
ASSERT_SIZE(SceneEnvironmentSetting, 0x3C);
} // namespace gfx
} // namespace nw
