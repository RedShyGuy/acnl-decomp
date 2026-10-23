#pragma once

#include "decomp.h"
#include "nn/math/math_Vector3.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResLookupTable.h"
#include "nw/gfx/res/gfx_ResSceneObject.h"
#include "nw/ut/ut_FloatColor.h"

namespace nw {
namespace gfx {
namespace res {
// The resources of the lights (CGFX "CFLT"/"CALT"/"CHLT"; only the fields known so far, names are
// ours): transform nodes with their colors.
struct ResLightData : public ResTransformNodeData
{
    enum Type
    {
        TYPE = 0x40000002,
    };

    bool isLightEnabled;     // 0xB4
    u8 m_Unknown0xB5[3];     // 0xB5
};
ASSERT_SIZE(ResLightData, 0xB8);

// a fragment light: with the lookup tables of the distance attenuation and the spot angle
struct ResFragmentLightData : public ResLightData
{
    enum Type
    {
        TYPE = 0x400000A2,
    };
    // lightKind
    enum LightKind
    {
        LIGHT_KIND_DIRECTIONAL = 0,
        LIGHT_KIND_POINT = 1,
        LIGHT_KIND_SPOT = 2,
    };
    // in flags (names are ours): both sides of a polygon are lit, the distance attenuation is used
    static const u32 FLAG_TWO_SIDED_DIFFUSE = 1 << 1;
    static const u32 FLAG_DISTANCE_ATTENUATION = 1 << 2;

    u32 lightKind;                    // 0xB8 (GetLightKind)
    ut::FloatColor ambient;           // 0xBC
    ut::FloatColor diffuse;           // 0xCC
    ut::FloatColor specular0;         // 0xDC
    ut::FloatColor specular1;         // 0xEC
    u32 ambientU8;                    // 0xFC the colors as ut::Color8 words
    u32 diffuseU8;                    // 0x100
    u32 specular0U8;                  // 0x104
    u32 specular1U8;                  // 0x108
    nn::math::VEC3 direction;         // 0x10C
    ut::Offset distanceSampler;       // 0x118 ResLookupTableData
    ut::Offset angleSampler;          // 0x11C ResLightingLookupTableData
    u32 m_Unknown0x120[2];            // 0x120
    u32 distanceAttenuationScale;     // 0x128 float20 (3dbrew "GPU/Internal Registers": LIGHTn_ATTENUATION_SCALE)
    u32 distanceAttenuationBias;      // 0x12C float20 (LIGHTn_ATTENUATION_BIAS)
    u8 m_Unknown0x130[4];             // 0x130
};
ASSERT_OFFSET(ResFragmentLightData, angleSampler, 0x11C);
ASSERT_SIZE(ResFragmentLightData, 0x134);

struct ResAmbientLightData : public ResLightData
{
    enum Type
    {
        TYPE = 0x40000422,
    };

    ut::FloatColor color;  // 0xB8
    u32 colorU32;          // 0xC8 the color as a PICA word
    u8 m_Unknown0xCC[4];   // 0xCC
};
ASSERT_SIZE(ResAmbientLightData, 0xD0);

struct ResHemiSphereLightData : public ResLightData
{
    enum Type
    {
        TYPE = 0x40000122,
    };
    // in flags: the direction turns with the light (if it has a parent; name is ours)
    static const u32 FLAG_DIRECTION_IN_WORLD = 1 << 1;

    ut::FloatColor groundColor;  // 0xB8
    ut::FloatColor skyColor;     // 0xC8
    nn::math::VEC3 direction;    // 0xD8
    f32 lerpFactor;              // 0xE4
};
ASSERT_SIZE(ResHemiSphereLightData, 0xE8);

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
    ResFragmentLightData* ptr() { return const_cast<ResFragmentLightData*>(static_cast<const ResFragmentLightData*>(m_pData)); }
    ResLookupTable GetDistanceSampler() const { return ResLookupTable(ptr()->distanceSampler.ToPtr()); }
    ResLightingLookupTable GetAngleSampler() const { return ResLightingLookupTable(ptr()->angleSampler.ToPtr()); }

    // the distance attenuation from start to end (at least 0.01 apart; name is ours)
    void SetDistanceAttenuation(f32 start, f32 end); // 0x004A76EC (name is ours)
};

// the binary resource of an ambient light (the class name is from the reference symbols)
class ResAmbientLight : public ResLight
{
public:
    // (inline)
    explicit ResAmbientLight(const void* pData = nullptr) : ResLight(pData) {}
    const ResAmbientLightData* ptr() const { return static_cast<const ResAmbientLightData*>(m_pData); }
};

// the binary resource of a hemisphere light (the class name is from the reference symbols)
class ResHemiSphereLight : public ResLight
{
public:
    // (inline)
    explicit ResHemiSphereLight(const void* pData = nullptr) : ResLight(pData) {}
    const ResHemiSphereLightData* ptr() const { return static_cast<const ResHemiSphereLightData*>(m_pData); }
};

} // namespace res
} // namespace gfx
} // namespace nw
