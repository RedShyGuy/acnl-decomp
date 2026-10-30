#pragma once

#include "decomp.h"

namespace mw {
namespace qrd {

// part of the camera image the decoder looks at (rows top..bottom, columns left..right); all zero = the
// whole image (names are ours)
struct Rect
{
    s32 top;    // +0x00
    s32 left;   // +0x04
    s32 right;  // +0x08
    s32 bottom; // +0x0C
};
ASSERT_SIZE(Rect, 0x10);

// camera image in YUYV (two bytes per pixel, the decoder reads the luminance bytes; names are ours)
struct ImageInfo
{
    u16 width;         // +0x00 at most 640
    u16 height;        // +0x02 at most 480
    const u8* pixels;  // +0x04
    Rect rect;         // +0x08
};
ASSERT_SIZE(ImageInfo, 0x18);

// what Decode found (names are ours)
struct DecodeResult
{
    s32 top;           // +0x00 bounding box of the code in the camera image
    s32 left;          // +0x04
    s32 right;         // +0x08
    s32 bottom;        // +0x0C
    s32 version;       // +0x10
    s32 dataSize;      // +0x14
    s32 appendIndex;   // +0x18 structured append: position of this symbol
    s32 appendCount;   // +0x1C structured append: number of symbols
    s32 appendParity;  // +0x20 structured append: parity of the whole data
    u8 dataMode;       // +0x24 DataMode
    u8 data[0x1BB2];   // +0x25 the data (7089 numeric characters at most) and a terminating 0
};
ASSERT_SIZE(DecodeResult, 0x1BD8);

// RTTI N2mw3qrd9QRDecoderE @ 0x008CDBB0
// vtable 0x008FBC04 (vptr 0x008FBC0C), offset_to_top 0, 2 entries
class QRDecoder
{
public:
    // mode of the decoded data (names are ours)
    enum DataMode : u8
    {
        DATA_MODE_NUMERIC = 0,
        DATA_MODE_ALPHANUMERIC = 1,
        DATA_MODE_KANJI = 2,
        DATA_MODE_8BIT = 3,
    };

    // return values of Decode (names are ours)
    enum Result
    {
        RESULT_SUCCESS = 0,
        RESULT_INVALID_ARGUMENT = 1,
        RESULT_NOT_FOUND = 2,
        RESULT_OUT_OF_MEMORY = 3,
        RESULT_VERSION_TOO_HIGH = 4,
        RESULT_NOT_INITIALIZED = 6,
    };

    QRDecoder();
    virtual ~QRDecoder();

    bool Initialize(void* work, u32 workSize); // 0x00344CB0 | fefates:bytes [tier B]
    void Finalize(); // 0x0012FD74 | fefates:bytes [tier B]
    s32 Decode(DecodeResult* result, const ImageInfo* image); // 0x00344D0C (name is ours)
    static u32 GetWorkSize(u32 width, u32 height); // 0x0034532C (name is ours)
};

} // namespace qrd
} // namespace mw
