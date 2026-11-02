#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_ArcResourceAccessor.h"
#include "ssys/ma/lyt/dTexBufList.h"

namespace ssys {
namespace ma {
namespace lyt {
// an archive accessor that copies the texture images (BCLIM) to VRAM, or uses them in place in device memory
// (function and member names are ours)
// RTTI N4ssys2ma3lyt23ArcResourceAccessorVRAME @ 0x008D2484
// vtable 0x009072EC (vptr 0x009072F4), offset_to_top 0, 7 entries
class ArcResourceAccessorVRAM : public ::nw::lyt::ArcResourceAccessor
{
public:
    // (inline in the constructor of ArcResAcc)
    ArcResourceAccessorVRAM() : m_Area(0x02010000) {}
    virtual ~ArcResourceAccessorVRAM(); // 0x00569024 slot 0x00
    nw::lyt::TextureInfo LoadTexture(const char* name) override; // 0x00568BF8 slot 0x14

    // the image of a BCLIM file in the memory area (0: VRAM A)
    nw::lyt::TextureInfo LoadTextureImage(const void* data, u32 size, u32 area); // 0x00568D90
    // frees the images and forgets the textures
    void ReleaseTextures(); // 0x00568CD0

    // (inline)
    void FreeImages();

    u32 m_Area;               // 0x13C the memory area of nngx (with flag bits 24-25)
    TexBufList m_TexBufList;  // 0x140
};
ASSERT_SIZE(ArcResourceAccessorVRAM, 0x14C);

// the formats of the BCLIM images: GL format and type and the smallest texture side (name is ours)
struct TexFormatInfo
{
    u32 format;     // 0x00 the BCLIM format
    u32 glFormat;   // 0x04
    u32 glType;     // 0x08
    u32 minSize;    // 0x0C
    u32 flags;      // 0x10
};
ASSERT_SIZE(TexFormatInfo, 0x14);
extern const TexFormatInfo s_TexFormatInfos[14];
} // namespace lyt
} // namespace ma
} // namespace ssys
