#pragma once

#include "decomp.h"

namespace mw {
namespace qre {

// what to encode (names are ours)
struct EncodeParam
{
    u32 size;          // +0x00
    const u8* data;    // +0x04
    s32 version;       // +0x08 0 = the smallest fitting
    s32 divideNum;     // +0x0C structured append: number of symbols (0 = as many as needed)
    s32 cellSize;      // +0x10 pixels per module
    u8 level;          // +0x14 error correction level (0 L, 1 M, 2 Q, 3 H)
};
ASSERT_SIZE(EncodeParam, 0x18);

// an encoded symbol as 24 bit RGB pixels (names are ours)
struct ImageInfo
{
    u16 width;         // +0x00
    u16 height;        // +0x02
    u8* pixels;        // +0x04
    f32 _08;           // +0x08
    f32 _0C;           // +0x0C
    f32 _10;           // +0x10
    f32 _14;           // +0x14
};
ASSERT_SIZE(ImageInfo, 0x18);

// method names are ours
// RTTI N2mw3qre9QREncoderE @ 0x008CDBB8
// vtable 0x008FBC14 (vptr 0x008FBC1C), offset_to_top 0, 2 entries
class QREncoder
{
public:
    QREncoder();
    virtual ~QREncoder();

    bool Initialize(void* work, u32 workSize);
    static u32 GetWorkSize(u32 dataSize, u32 cellSize, u32 divideNum);
    bool Encode(EncodeParam* param);
    bool GetImage(ImageInfo* image, s32 index);
    u32 GetImageSize(s32 index, bool isBmp);
};

} // namespace qre
} // namespace mw
