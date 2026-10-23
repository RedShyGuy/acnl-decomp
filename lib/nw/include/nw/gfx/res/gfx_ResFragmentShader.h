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
        FLAG_CLAMP_HIGHLIGHTS = 0x1,
        FLAG_SAMPLER3 = 0x2,         // sampler 3 is used (distribution 0)
        FLAG_SAMPLER4 = 0x4,         // sampler 4 is used (distribution 1)
        FLAG_GEOMETRIC_FACTOR0 = 0x8,
        FLAG_GEOMETRIC_FACTOR1 = 0x10,
        FLAG_CONFIG_SAMPLERS = 0x20, // the samplers of the configuration are used (reflection)
    };

    u32 flags;             // 0x00
    u32 config;            // 0x04 the lighting configuration (index of the sampler table)
    u32 fresnelSelector;   // 0x08 where the Fresnel table (sampler 5) applies (0: nowhere)
    u32 bumpSelector;      // 0x0C the texture unit of the bump map
    u32 bumpMode;          // 0x10
    bool isBumpRenormalize; // 0x14
    u8 m_Unknown0x15[3];   // 0x15
};
ASSERT_SIZE(ResFragmentLightingData, 0x18);

// a stage of the texture combiners: the constant color it uses and its commands (TEXENVn_*; names are ours)
struct ResTextureCombinerData
{
    u32 constantSource;  // 0x00 the color of the material it uses as constant
    u32 command[6];      // 0x04 SOURCE, header, OPERAND, COMBINER, COLOR, SCALE
};
ASSERT_SIZE(ResTextureCombinerData, 0x1C);

// the wrapper of the fragment lighting settings (the class name is from the reference symbols)
class ResFragmentLighting
{
public:
    // (inline)
    explicit ResFragmentLighting(const void* pData = nullptr) : m_pData(static_cast<const ResFragmentLightingData*>(pData)) {}
    const ResFragmentLightingData* ptr() const { return m_pData; }

    const ResFragmentLightingData* m_pData; // 0x0
};

// a fragment shader of a material (only the fields known so far; names are ours)
struct ResFragmentShaderData
{
    u32 typeInfo;                     // 0x00
    u32 m_Unknown0x4[3];              // 0x04
    ResFragmentLightingData lighting; // 0x10
    ut::Offset lightingTable;         // 0x28 ResFragmentLightingTableData
    ResTextureCombinerData combiners[6]; // 0x2C
    u32 bufferCommand[2];             // 0xD4 (TEXENV_UPDATE_BUFFER)
    u32 bufferColorCommand[6];        // 0xDC
};
ASSERT_OFFSET(ResFragmentShaderData, lightingTable, 0x28);
ASSERT_SIZE(ResFragmentShaderData, 0xF4);

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
