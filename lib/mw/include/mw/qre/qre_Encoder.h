#pragma once

#include "decomp.h"
#include "mw/qre/QREncoder.h"
#include "mw/qre/qre_Internal.h"

namespace mw {
namespace qre {
namespace detail {

// the data split over the symbols of a structured append series
struct DivideInfo
{
    s32 count;      // +0x00
    Part* parts;    // +0x04
    DataSpec data;  // +0x08 a copy of the input
    u8 method;      // +0x10 0 = one symbol, 1 = split evenly
};
ASSERT_SIZE(DivideInfo, 0x14);

// the encoder behind QREncoder (one global instance; all names are ours)
class Encoder
{
public:
    Encoder();
    ~Encoder();

    bool Encode(EncodeParam* param);
    bool GetImage(ImageInfo* image, s32 index);
    u32 GetImageSize(s32 index, bool isBmp);
    void ClearImages();

private:
    bool CalcDivide(EncodeParam* param, s32* divideNum, s32* version, s32 mode);
    bool RenderImage(s32 size, s32 cellSize, s32 quietZone, u8** matrix, s32 index);
    bool EncodePart(Part* part, s32 index);
    void SetErrorMessage(const char* detail);
    bool Divide(DivideInfo* info, EncodeParam* param, s32 mode);

    ImageInfo m_Images[16];  // +0x000
    char m_Message[256];     // +0x180
    s32 m_DivideNum;         // +0x280
    s32 m_Version;           // +0x284
    s32 m_ImageWidth;        // +0x288 pixels per side
    u8 m_IsEncoded;          // +0x28C
};

extern Encoder s_Encoder;

} // namespace detail
} // namespace qre
} // namespace mw
