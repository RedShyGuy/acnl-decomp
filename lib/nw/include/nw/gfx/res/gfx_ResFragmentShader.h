#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResFragmentLightingTable.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
struct ResMaterialData;

// The fragment lighting settings of a fragment shader (names are ours): which lookup tables the
// lighting uses. A setting whose sampler has no table is turned off by CheckFragmentShader.
struct ResFragmentLightingData
{
    enum Flag
    {
        FLAG_SAMPLER3 = 0x2,         // sampler 3 is used
        FLAG_SAMPLER4 = 0x4,         // sampler 4 is used
        FLAG_CONFIG_SAMPLERS = 0x20, // the samplers of the configuration are used
    };

    u32 flags;           // 0x0
    u32 config;          // 0x4 the lighting configuration (index of the sampler table)
    u32 isSampler5Used;  // 0x8 (a bool)
};
ASSERT_SIZE(ResFragmentLightingData, 0xC);

// a fragment shader of a material (only the fields known so far; names are ours)
struct ResFragmentShaderData
{
    u32 typeInfo;                     // 0x00
    u32 m_Unknown0x4[3];              // 0x04
    ResFragmentLightingData lighting; // 0x10
    u32 m_Unknown0x1C[3];             // 0x1C
    ut::Offset lightingTable;         // 0x28 ResFragmentLightingTableData
};
ASSERT_OFFSET(ResFragmentShaderData, lightingTable, 0x28);

// the binary resource of a fragment shader (the class name and the functions are from the reference
// symbols)
class ResFragmentShader
{
public:
    // (inline)
    explicit ResFragmentShader(const void* pData = nullptr) : m_pData(static_cast<const ResFragmentShaderData*>(pData)) {}
    const ResFragmentShaderData* ptr() const { return m_pData; }
    ResFragmentShaderData* ptr() { return const_cast<ResFragmentShaderData*>(m_pData); }
    bool IsValid() const { return m_pData != nullptr; }
    ResFragmentLightingTable GetLightingTable() const { return ResFragmentLightingTable(m_pData->lightingTable.ToPtr()); }

    void Cleanup(); // 0x0013F170 | nintendogs:bytes-fuzzy [tier A]
    // the lighting settings whose tables are missing turned off; returns whether all were there
    bool CheckFragmentShader(ResMaterialData* pMaterial); // 0x004A7984 | nintendogs:bytes [tier B]
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile, ResMaterialData* pMaterial); // 0x004A7BF4 | nintendogs:bytes [tier B]

    const ResFragmentShaderData* m_pData; // 0x0
};
ASSERT_SIZE(ResFragmentShader, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
