#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/gfx/res/gfx_ResTextureSampler.h"
#include "nw/os/os_MemorySizeCalculator.h"

namespace nw {
namespace gfx {
namespace res {
// A texture mapper: the texture of a texture unit (only the fields known so far; names are ours). A
// clone made at runtime (CloneDynamic) has its allocator and a reference texture of its own.
struct ResTextureMapperData
{
    u32 typeInfo;                       // 0x0
    os::IAllocator* pDynamicAllocator;  // 0x4 (a clone: the allocator of its memory)
    ut::Offset texture;                 // 0x8 ResTextureData (a clone: a ResReferenceTextureData)
};
ASSERT_SIZE(ResTextureMapperData, 0xC);

// a mapper of a pixel-based texture: with its sampler and the PICA commands of texture unit 0
// (3dbrew "GPU/Internal Registers": TEXUNIT0_*)
struct ResPixelBasedTextureMapperData : public ResTextureMapperData
{
    // the words of the commands
    enum Command
    {
        COMMAND_TYPE = 0,           // the format
        COMMAND_TYPE_HEADER = 1,
        COMMAND_BORDER_COLOR = 2,
        COMMAND_BORDER_COLOR_HEADER = 3, // the registers from BORDER_COLOR on (4: up to ADDR1, 9: ADDR6, 10: SHADOW)
        COMMAND_DIM = 4,            // height | width << 16
        COMMAND_PARAM = 5,
        COMMAND_LOD = 6,            // bits 16-19: the highest mipmap level
        COMMAND_ADDR1 = 7,          // the physical address / 8
        COMMAND_ADDR2 = 8,          // ADDR2-6: the other faces of a cube
        COMMAND_SHADOW = 13,        // bit 0: perspective, bits 1-23: the bias
        COMMAND_COUNT = 14,
    };

    ut::Offset sampler;              // 0x0C ResTextureSamplerData
    u32 commands[COMMAND_COUNT];     // 0x10
    u32 commandSize;                 // 0x48 bytes
};
ASSERT_SIZE(ResPixelBasedTextureMapperData, 0x4C);

// a mapper of a procedural texture
struct ResProceduralTextureMapperData : public ResTextureMapperData
{
};
ASSERT_SIZE(ResProceduralTextureMapperData, 0xC);

// the binary resource of a texture mapper (the class name and the functions are from the
// reference symbols)
class ResTextureMapper
{
public:
    enum Type
    {
        TYPE_PIXEL_BASED = 0x80000000,
        TYPE_PROCEDURAL = 0x40000000,
    };

    // (inline)
    explicit ResTextureMapper(const void* pData = nullptr) : m_pData(static_cast<const ResTextureMapperData*>(pData)) {}
    const ResTextureMapperData* ptr() const { return m_pData; }
    ResTextureMapperData* ptr() { return const_cast<ResTextureMapperData*>(m_pData); }
    bool IsValid() const { return m_pData != nullptr; }
    ResTexture GetTexture() const { return ResTexture(m_pData->texture.ToPtr()); }

    void Cleanup(); // 0x0013F13C | nintendogs:callgraph [tier A]
    // the texture of the mapper (its reference texture refers to it)
    void SetTexture(ResTexture texture); // 0x004A77D0 | nintendogs:bytes [tier A]
    // a copy in memory of the allocator (NULL: no memory)
    ResTextureMapper CloneDynamic(os::IAllocator* pAllocator); // 0x004A7894 | nintendogs:bytes [tier A]
    // a clone freed
    void DestroyDynamic(); // 0x004A78F4 | nintendogs:bytes [tier A]
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004A7934 | nintendogs:bytes [tier A]
    // the memory CloneDynamic allocates
    DECOMP_NOINLINE void GetMemorySizeForCloneInternal(os::MemorySizeCalculator* pSize) const // 0x0073C724 | nintendogs:bytes [tier A]
    {
        if (m_pData->typeInfo == TYPE_PIXEL_BASED) {
            pSize->Add(sizeof(ResPixelBasedTextureMapperData));
            pSize->Add(sizeof(ResReferenceTextureData));
            pSize->Add(sizeof(ResTextureSamplerData));
        }
    }

    const ResTextureMapperData* m_pData; // 0x0
};
ASSERT_SIZE(ResTextureMapper, 0x4);

// the binary resource of a pixel-based texture mapper (the class name and the functions are from the
// reference symbols)
class ResPixelBasedTextureMapper : public ResTextureMapper
{
public:
    // (inline)
    explicit ResPixelBasedTextureMapper(const void* pData = nullptr) : ResTextureMapper(pData) {}
    const ResPixelBasedTextureMapperData* ptr() const { return static_cast<const ResPixelBasedTextureMapperData*>(m_pData); }
    ResPixelBasedTextureMapperData* ptr()
    {
        return const_cast<ResPixelBasedTextureMapperData*>(static_cast<const ResPixelBasedTextureMapperData*>(m_pData));
    }

    // the texture set and its commands built (without the setup of the texture)
    void ForceSetupTexture(ResTexture texture); // 0x004A9630 | mk7dlp:bytes [tier B]
    void DestroyDynamic(); // 0x004A9CE0 | nintendogs:bytes [tier B]
};

namespace internal {
// the texture-unit commands of a mapper for a texture of each kind (names are ours)
void BuildImageTextureCommand(ResPixelBasedTextureMapperData* pMapper, const ResTextureData* pTexture);
void BuildCubeTextureCommand(ResPixelBasedTextureMapperData* pMapper, const ResTextureData* pTexture);
void BuildShadowTextureCommand(ResPixelBasedTextureMapperData* pMapper, const ResTextureData* pTexture);
// the commands for the texture, by its kind
inline void BuildTextureCommand(ResPixelBasedTextureMapperData* pMapper, const ResTextureData* pTexture)
{
    switch (pTexture->typeInfo) {
    case ResTexture::TYPE_CUBE:
        BuildCubeTextureCommand(pMapper, pTexture);
        break;
    case ResTexture::TYPE_IMAGE:
        BuildImageTextureCommand(pMapper, pTexture);
        break;
    case ResTexture::TYPE_SHADOW:
        BuildShadowTextureCommand(pMapper, pTexture);
        break;
    default:
        break;
    }
}
// (inline) the texture a texture refers to (itself if it is no reference)
inline const ResTextureData* DereferenceTexture(const ResTextureData* pTexture)
{
    if (pTexture->typeInfo == ResTexture::TYPE_REFERENCE) {
        return static_cast<const ResReferenceTextureData*>(pTexture)->target.To<ResTextureData>();
    }
    return pTexture;
}
// the destruction of a clone of each kind
void DestroyPixelBasedTextureMapper(ResTextureMapperData* pMapper, os::IAllocator* pAllocator);
} // namespace internal

// (inline; declared in gfx_ResTextureSampler.h)
DECOMP_EMIT_COPY inline OwnerCommand ResTextureSampler::GetOwnerCommand() const
{
    const ResPixelBasedTextureMapperData* pOwner = m_pData->owner.To<ResPixelBasedTextureMapperData>();
    OwnerCommand command;
    command.size = pOwner->commandSize;
    command.pCommands = const_cast<u32*>(pOwner->commands);
    return command;
}
} // namespace res
} // namespace gfx
} // namespace nw
