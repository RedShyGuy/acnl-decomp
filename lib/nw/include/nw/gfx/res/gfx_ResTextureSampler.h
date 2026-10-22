#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"

namespace nw {
namespace gfx {
namespace res {
// How a texture mapper samples its texture (only the fields known so far; names are ours). The
// filters and wraps are written into the PICA commands of the owning mapper.
struct ResTextureSamplerData
{
    // the minification filter
    enum MinFilter
    {
        MIN_FILTER_NEAREST = 0,
        MIN_FILTER_LINEAR = 1,
        // 2-5: with mipmaps
    };

    u32 typeInfo;          // 0x00
    ut::Offset owner;      // 0x04 the ResPixelBasedTextureMapperData
    u32 minFilter;         // 0x08 MinFilter (low byte)
    u32 m_Unknown0xC[5];   // 0x0C
};
ASSERT_SIZE(ResTextureSamplerData, 0x20);

// the commands of the owning mapper (its size in bytes and the commands; names are ours)
struct OwnerCommand
{
    u32 size;        // 0x0
    u32* pCommands;  // 0x4
};

// the binary resource of a texture sampler (the class name and the functions are from the
// reference symbols)
class ResTextureSampler
{
public:
    enum Type
    {
        TYPE_STANDARD = 0x80000000,
    };

    // (inline)
    explicit ResTextureSampler(const void* pData = nullptr) : m_pData(static_cast<const ResTextureSamplerData*>(pData)) {}
    const ResTextureSamplerData* ptr() const { return m_pData; }
    ResTextureSamplerData* ptr() { return const_cast<ResTextureSamplerData*>(m_pData); }
    bool IsValid() const { return m_pData != nullptr; }

    // the highest mipmap level in the LOD command of the owner (0 without a mipmap filter)
    void SetTextureMipmapCommand(); // 0x004A7D08 | nintendogs:bytes [tier B]
    OwnerCommand GetOwnerCommand() const; // 0x0073C788 | nintendogs:bytes [tier A] (inline, in gfx_ResTextureMapper.h)

    const ResTextureSamplerData* m_pData; // 0x0
};
ASSERT_SIZE(ResTextureSampler, 0x4);
} // namespace res
} // namespace gfx
} // namespace nw
