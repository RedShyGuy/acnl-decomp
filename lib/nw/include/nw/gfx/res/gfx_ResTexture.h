#pragma once

#include "decomp.h"
#include "nw/gfx/res/gfx_ResCommon.h"
#include "nw/gfx/res/gfx_ResGraphicsFile.h"

namespace nw {
namespace gfx {
namespace res {
// A texture (3dbrew "CGFX": TXOB; only the fields known so far, names ours). The pixel-based kinds
// (image, cube, shadow) have their images uploaded by Setup: to the area of the location flag (VRAM A
// or B, a copy in FCRAM, or the data in place).
struct ResTextureData
{
    u32 typeInfo;          // 0x00
    u32 signature;         // 0x04 "TXOB"
    u32 m_Unknown0x8;      // 0x08
    ut::Offset name;       // 0x0C
    s32 userDataCount;     // 0x10
    ut::Offset userDataDic; // 0x14
};
ASSERT_SIZE(ResTextureData, 0x18);

// an image of a pixel-based texture
struct ResImageData
{
    u32 height;            // 0x00
    u32 width;             // 0x04
    u32 size;              // 0x08 bytes (0: height * width * bitsPerPixel / 8)
    ut::Offset data;       // 0x0C
    os::IAllocator* pAllocator; // 0x10 (an image of ResImageTexture::CreateDynamic) the allocator of the image
    u32 bitsPerPixel;      // 0x14
    void* pLocationAddress; // 0x18 the uploaded copy (NULL: the data itself)
    u32 locationArea;      // 0x1C the area of the copy
};
ASSERT_SIZE(ResImageData, 0x20);

struct ResPixelBasedTextureData : public ResTextureData
{
    u32 height;            // 0x18
    u32 width;             // 0x1C
    u32 glFormat;          // 0x20 the format of the pixels for GL
    u32 glType;            // 0x24 the type of the pixels for GL (0: compressed)
    u32 mipmapLevels;      // 0x28
    const void* pLoaded;   // 0x2C the texture itself once its images are uploaded (the name for the allocator)
    u32 locationFlag;      // 0x30 where the images go (0: LOCATION_NO_COPY)
    u32 format;            // 0x34 (3dbrew "CGFX": the texture formats; FORMAT_ETC1 = 12)
};
ASSERT_SIZE(ResPixelBasedTextureData, 0x38);

struct ResImageTextureData : public ResPixelBasedTextureData
{
    ut::Offset image; // 0x38 ResImageData
};
ASSERT_SIZE(ResImageTextureData, 0x3C);

struct ResCubeTextureData : public ResPixelBasedTextureData
{
    ut::Offset images[6]; // 0x38 ResImageData per face
};

struct ResShadowTextureData : public ResPixelBasedTextureData
{
    ut::Offset image;    // 0x38 ResImageData
    bool isOrthographic; // 0x3C (else the shadow is perspective)
    f32 bias;            // 0x40
};
ASSERT_OFFSET(ResShadowTextureData, bias, 0x40);

// a texture referenced by name (a texture of the file)
struct ResReferenceTextureData : public ResTextureData
{
    ut::Offset path;   // 0x18 the name of the texture
    ut::Offset target; // 0x1C the texture, once resolved
};
ASSERT_SIZE(ResReferenceTextureData, 0x20);

// the binary resource of a texture (the class name and Setup / Cleanup are from the reference symbols)
class ResTexture
{
public:
    enum Type
    {
        TYPE_PROCEDURAL = 0x20000002,
        TYPE_REFERENCE = 0x20000004,
        TYPE_CUBE = 0x20000009,
        TYPE_IMAGE = 0x20000011,
        TYPE_SHADOW = 0x20000021,
    };
    enum Format
    {
        FORMAT_ETC1 = 12,
    };
    // the location flag: where the images are uploaded (high half: the area of the allocator)
    enum Location
    {
        LOCATION_FCRAM_COPY = 0x01010000,
        LOCATION_NO_COPY = 0x02010000,
        LOCATION_VRAM_A = 0x02020000,
        LOCATION_VRAM_B = 0x02030000,
    };

    // (inline)
    explicit ResTexture(const void* pData = nullptr) : m_pData(static_cast<const ResTextureData*>(pData)) {}
    const ResTextureData* ptr() const { return m_pData; }
    bool IsValid() const { return m_pData != nullptr; }
    u32 GetTypeInfo() const { return m_pData->typeInfo; }
    // the name (NULL: none) for a texture the game made; returns the texture (name is ours; the
    // original loads the data pointer as the result and then the type word, which it does not use)
    ResTexture SetName(const char* name); // 0x004A8718

    void Cleanup(); // 0x001375CC | nintendogs:bytes-fuzzy [tier A]
    u32 Setup(os::IAllocator* pAllocator, ResGraphicsFile graphicsFile); // 0x004A5AB8 | nintendogs:bytes [tier A]

    const ResTextureData* m_pData; // 0x0
};
ASSERT_SIZE(ResTexture, 0x4);

class ResReferenceTexture : public ResTexture
{
public:
    // (inline)
    explicit ResReferenceTexture(const void* pData = nullptr) : ResTexture(pData) {}
    const ResReferenceTextureData* ptr() const { return static_cast<const ResReferenceTextureData*>(m_pData); }
    ResReferenceTextureData* ptr() { return const_cast<ResReferenceTextureData*>(static_cast<const ResReferenceTextureData*>(m_pData)); }
};

// a texture of one image (class name is ours)
class ResImageTexture : public ResTexture
{
public:
    // a texture made without a file (names are ours)
    struct DynamicDescription
    {
        s32 height;              // 0x00
        s32 width;               // 0x04
        s32 mipmapLevels;        // 0x08 at most (fewer if the image gets to the smallest size)
        u32 locationFlag;        // 0x0C
        void* pLocationAddress;  // 0x10 the memory of the image (NULL: none yet)
        u32 format;              // 0x14 (3dbrew "CGFX": the texture formats)
        bool isCleared;          // 0x18 the allocated pixels set to 0
        bool isAllocated;        // 0x19 memory allocated for the pixels
    };

    // (inline)
    explicit ResImageTexture(const void* pData = nullptr) : ResTexture(pData) {}
    const ResImageTextureData* ptr() const { return static_cast<const ResImageTextureData*>(m_pData); }
    ResImageTextureData* ptr() { return const_cast<ResImageTextureData*>(static_cast<const ResImageTextureData*>(m_pData)); }

    // a texture with its image (and the memory of the pixels) from the allocator (name is ours)
    static ResImageTexture CreateDynamic(const DynamicDescription& description, os::IAllocator* pAllocator); // 0x004A72B0 (name is ours)
    // the texture of CreateDynamic freed (name is ours)
    void DestroyDynamic(); // 0x004A7530 (name is ours)
};
ASSERT_SIZE(ResImageTexture, 0x4);

// the texture of the name in the graphics file (a reference: the texture it refers to)
ReferenceTarget<ResTexture> GetReferenceTextureTarget(ResReferenceTexture texture, ResGraphicsFile graphicsFile); // 0x004A8D28 | nintendogs:bytes [tier A]
} // namespace res
} // namespace gfx
} // namespace nw
