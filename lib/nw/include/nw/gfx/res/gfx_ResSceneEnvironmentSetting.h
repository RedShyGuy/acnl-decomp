#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"

namespace nw {
namespace gfx {
namespace res {
// A light set of a scene environment setting (only the fields known so far; names are ours).
struct ResLightSetData
{
    u32 m_Unknown0x0; // 0x0
    s32 lightCount;   // 0x4 (the room for the lights of the light set)
};

// the binary resource of a light set (the class name is from the reference symbols)
class ResLightSet
{
public:
    // (inline)
    explicit ResLightSet(const void* pData = nullptr) : m_pData(static_cast<const ResLightSetData*>(pData)) {}
    const ResLightSetData* ptr() const { return m_pData; }

    const ResLightSetData* m_pData; // 0x0
};
ASSERT_SIZE(ResLightSet, 0x4);

// The environment setting of a scene (CGFX "CENV"; only the fields known so far, names are ours): the
// light sets, cameras and fogs a scene uses.
// (the header of a scene object without the flags and the rest)
struct ResSceneEnvironmentSettingData
{
    enum Type
    {
        TYPE = 0x00800000,
    };

    u32 typeInfo;            // 0x00
    u32 signature;           // 0x04
    u32 m_Unknown0x8;        // 0x08
    ut::Offset name;         // 0x0C
    s32 userDataCount;       // 0x10
    ut::Offset userDataDic;  // 0x14
    ResArrayInfo cameras;    // 0x18
    ResArrayInfo lightSets;  // 0x20 ResLightSetData
    ResArrayInfo fogs;       // 0x28
};
ASSERT_SIZE(ResSceneEnvironmentSettingData, 0x30);

// the binary resource of a scene environment setting (the class name is from the reference symbols)
class ResSceneEnvironmentSetting : public ResSceneObject
{
public:
    // (inline)
    explicit ResSceneEnvironmentSetting(const void* pData = nullptr) : ResSceneObject(pData) {}
    const ResSceneEnvironmentSettingData* ptr() const { return reinterpret_cast<const ResSceneEnvironmentSettingData*>(m_pData); }
};
ASSERT_SIZE(ResSceneEnvironmentSetting, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
