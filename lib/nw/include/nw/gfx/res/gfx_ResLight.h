#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"

namespace nw {
namespace gfx {
namespace res {
// A light (only the fields known so far; names are ours).
struct ResLightData
{
    u32 typeInfo; // 0x0
};

// a fragment light: with the lookup tables of the distance attenuation and the spot angle
struct ResFragmentLightData : public ResLightData
{
    u8 m_Unknown0x4[0x114];      // 0x4
    ut::Offset distanceSampler;  // 0x118 ResLookupTableData
    ut::Offset angleSampler;     // 0x11C ResLightingLookupTableData
};
ASSERT_OFFSET(ResFragmentLightData, angleSampler, 0x11C);

// the binary resource of a light (the class name is from the reference symbols)
class ResLight
{
public:
    enum Type
    {
        TYPE_FRAGMENT_LIGHT = 0x400000A2,
    };

    // (inline)
    explicit ResLight(const void* pData = nullptr) : m_pData(static_cast<const ResLightData*>(pData)) {}
    const ResLightData* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }

    void Cleanup(); // 0x001376F4 | nintendogs:bytes [tier A]
    // the lookup tables of a fragment light
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004AA2E8 | nintendogs:bytes [tier A]

    const ResLightData* m_pData; // 0x0
};
ASSERT_SIZE(ResLight, 0x4);

// the binary resource of a fragment light (the class name is from the reference symbols)
class ResFragmentLight : public ResLight
{
public:
    // (inline)
    explicit ResFragmentLight(const void* pData = nullptr) : ResLight(pData) {}
    const ResFragmentLightData* ptr() const { return static_cast<const ResFragmentLightData*>(m_pData); }
    ResLookupTable GetDistanceSampler() const { return ResLookupTable(ptr()->distanceSampler.ToPtr()); }
    ResLightingLookupTable GetAngleSampler() const { return ResLightingLookupTable(ptr()->angleSampler.ToPtr()); }
};
} // namespace res
} // namespace gfx
} // namespace nw
