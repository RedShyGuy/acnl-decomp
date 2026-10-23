#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"
#include "nw/ut/ut_FloatColor.h"

namespace nw {
namespace gfx {
namespace res {
// How the fog grows with the depth (names are ours).
struct ResFogUpdaterData
{
    enum FogType : u8
    {
        FOG_TYPE_NONE = 0,
        FOG_TYPE_LINEAR = 1,
        FOG_TYPE_EXPONENT = 2,
        FOG_TYPE_EXPONENT_SQUARE = 3,
    };

    u32 fogType;   // 0x0 FogType (low byte)
    f32 start;     // 0x4 depth where the fog starts
    f32 end;       // 0x8 depth where everything is fog
    f32 density;   // 0xC (the exponent types)
};
ASSERT_SIZE(ResFogUpdaterData, 0x10);

// A fog (3dbrew "CGFX": CFOG; only the fields known so far, names are ours): a transform node with
// the color, the lookup table of the fog (filled by Fog::Update) and its updater.
struct ResFogData : public ResTransformNodeData
{
    enum Type
    {
        TYPE = 0x40000042,
    };
    // the header of a resource made by the DynamicBuilder ("CFOG")
    static const u32 SIGNATURE = 0x474F4643;
    static const u32 DYNAMIC_REVISION = 0x06000000;
    // in flags: the fog is flipped in z (name is ours)
    static const u32 FLAG_Z_FLIP = 1 << 1;

    ut::FloatColor color;  // 0xB4
    ut::Offset sampler;    // 0xC4 ResImageLookupTableData
    ut::Offset updater;    // 0xC8 ResFogUpdaterData
};
ASSERT_OFFSET(ResFogData, sampler, 0xC4);
ASSERT_SIZE(ResFogData, 0xCC);

// the binary resource of the updater of a fog (the class name is from the reference symbols)
class ResFogUpdater
{
public:
    // (inline)
    explicit ResFogUpdater(const void* pData = nullptr) : m_pData(static_cast<const ResFogUpdaterData*>(pData)) {}
    const ResFogUpdaterData* ptr() const { return m_pData; }

    const ResFogUpdaterData* m_pData; // 0x0
};
ASSERT_SIZE(ResFogUpdater, 0x4);

// the binary resource of a fog (the class name and the functions are from the reference symbols)
class ResFog : public ResTransformNode
{
public:
    // (inline)
    explicit ResFog(const void* pData = nullptr) : ResTransformNode(pData) {}
    const ResFogData* ptr() const { return static_cast<const ResFogData*>(m_pData); }
    bool IsValid() const { return m_pData != nullptr; }
    ResLookupTable GetSampler() const { return ResLookupTable(ptr()->sampler.ToPtr()); }

    void Cleanup(); // 0x001376C8 | nintendogs:bytes [tier A]
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004A9DEC | nintendogs:bytes [tier A]
};
ASSERT_SIZE(ResFog, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
