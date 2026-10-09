// The C writers of the decoder: they convert a decoded MCU (YCbCr, 8x8 blocks) into the output
// format. The "Write" functions are used when the image size is not a multiple of the MCU size
// (the assembly writes whole MCUs only), the "Shrink" functions when the image is scaled down;
// they take every (1 << shrink)-th sample. The functions are listed in the order of the binary.

#include <arm_acle.h>
#include "nn/jpeg/CTR/detail/detail_Api.h"

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail {
namespace {
// the offsets of the pixels of an 8x8 tile of the block formats (Morton order), for 2, 3 and 4
// byte pixels; the 2 byte table counts pixels
// 0x0089EA00
const u8 s_Block8Index[64] = {
    0,  1,  4,  5,  16, 17, 20, 21, 2,  3,  6,  7,  18, 19, 22, 23, 8,  9,  12, 13, 24, 25, 28, 29, 10, 11, 14, 15, 26, 27, 30, 31,
    32, 33, 36, 37, 48, 49, 52, 53, 34, 35, 38, 39, 50, 51, 54, 55, 40, 41, 44, 45, 56, 57, 60, 61, 42, 43, 46, 47, 58, 59, 62, 63,
};
// 0x0089EA40
const u8 s_Block8Offset3[64] = {
    0,  3,  12,  15,  48,  51,  60,  63,  6,   9,   18,  21,  54,  57,  66,  69,  24,  27,  36,  39,  72,  75,
    84, 87, 30,  33,  42,  45,  78,  81,  90,  93,  96,  99,  108, 111, 144, 147, 156, 159, 102, 105, 114, 117,
    150, 153, 162, 165, 120, 123, 132, 135, 168, 171, 180, 183, 126, 129, 138, 141, 174, 177, 186, 189,
};
// 0x0089EA80
const u8 s_Block8Offset4[64] = {
    0,   4,   16,  20,  64,  68,  80,  84,  8,   12,  24,  28,  72,  76,  88,  92,  32,  36,  48,  52,  96,  100,
    112, 116, 40,  44,  56,  60,  104, 108, 120, 124, 128, 132, 144, 148, 192, 196, 208, 212, 136, 140, 152, 156,
    200, 204, 216, 220, 160, 164, 176, 180, 224, 228, 240, 244, 168, 172, 184, 188, 232, 236, 248, 252,
};

// YCbCr to RGB (ITU-R BT.601, fixed point)
inline int ToRed(int y, int cr)
{
    return y + (((cr - 128) * 22971 + 8192) >> 14);
}

inline int ToGreen(int y, int cb, int cr)
{
    return y + (((cb - 128) * -11277 + (cr - 128) * -23401 + 16384) >> 15);
}

inline int ToBlue(int y, int cb)
{
    return y + (((cb - 128) * 29033 + 8192) >> 14);
}

// (the saturation is the usat instruction, also with the shift of the 5/6 bit values)
inline u8 Saturate(int value)
{
    return __usat(value, 8);
}

inline u16 ToRgb565(int y, int cb, int cr)
{
    return __usat(ToRed(y, cr) >> 3, 5) << 11 | __usat(ToGreen(y, cb, cr) >> 2, 6) << 5 | __usat(ToBlue(y, cb) >> 3, 5);
}

// The output formats. GetMcu returns where the MCU (or the scaled down MCU at sx, sy) starts,
// GetOffset/GetShrunkOffset where a pixel of it goes and Put writes the pixel there.

// lines of pixels of BYTES bytes
template <int BYTES>
struct LinearLayout
{
    static u8* GetMcu(const JpegMpDecoderContext* context, int x, int y)
    {
        return static_cast<u8*>(context->m_Dst) + (y * context->m_Stride + x) * BYTES;
    }

    static u32 GetOffset(u32 stride, int px, int py, int, int, int, int)
    {
        return (py * stride + px) * BYTES;
    }

    static u32 GetShrunkOffset(u32 stride, int, int, int col, int row)
    {
        return (row * stride + col) * BYTES;
    }
};

// 8x8 tiles of pixels of BYTES bytes, the pixels of a tile in Morton order
template <int BYTES>
struct Block8Layout
{
    static u8* GetMcu(const JpegMpDecoderContext* context, int x, int y)
    {
        return static_cast<u8*>(context->m_Dst) + (y * context->m_Stride + x * 8) * BYTES;
    }

    static u8* GetShrunkMcu(const JpegMpDecoderContext* context, int x, int y)
    {
        return static_cast<u8*>(context->m_Dst) + ((y & ~7) * context->m_Stride + (x & ~7) * 8) * BYTES;
    }

    static u32 GetTileOffset(int index)
    {
        return BYTES == 2 ? s_Block8Index[index] * 2 : BYTES == 3 ? s_Block8Offset3[index] : s_Block8Offset4[index];
    }

    static u32 GetOffset(u32 stride, int, int, int bx, int by, int x, int y)
    {
        return (by * 8 * stride + bx * 64) * BYTES + GetTileOffset(y * 8) + GetTileOffset(x);
    }

    static u32 GetShrunkOffset(u32, int sx, int sy, int col, int row)
    {
        return GetTileOffset(((sy & 7) + row) * 8 + (sx & 7) + col);
    }
};

template <int BYTES>
struct LinearShrinkLayout : LinearLayout<BYTES>
{
    static u8* GetShrunkMcu(const JpegMpDecoderContext* context, int x, int y)
    {
        return LinearLayout<BYTES>::GetMcu(context, x, y);
    }
};

// Y0 Cb Y1 Cr: the even pixel writes its Y and the chroma, the odd one adds its Y. The offsets
// are pixels (the parity of the pixel decides).
struct Yuyv8Format
{
    static u8* GetMcu(const JpegMpDecoderContext* context, int x, int y)
    {
        return static_cast<u8*>(context->m_Dst) + (y * context->m_Stride + x) * 2;
    }

    static u8* GetShrunkMcu(const JpegMpDecoderContext* context, int, int y)
    {
        return static_cast<u8*>(context->m_Dst) + y * context->m_Stride * 2;
    }

    static u32 GetOffset(u32 stride, int px, int py, int, int, int, int)
    {
        return py * stride + px;
    }

    static u32 GetShrunkOffset(u32 stride, int sx, int, int col, int row)
    {
        return row * stride + sx + col;
    }

    static void Put(u8* mcu, u32 index, int y, int cb, int cr)
    {
        u16* dst = reinterpret_cast<u16*>(mcu);
        if (index & 1) {
            dst[index] |= y;
        } else {
            dst[index] = y | (cb << 8);
            dst[index + 1] = static_cast<u16>(cr << 8);
        }
    }
};

struct Rgb565Format : LinearShrinkLayout<2>
{
    static void Put(u8* mcu, u32 offset, int y, int cb, int cr)
    {
        *reinterpret_cast<u16*>(mcu + offset) = ToRgb565(y, cb, cr);
    }
};

struct Rgb565Block8Format : Block8Layout<2>
{
    static void Put(u8* mcu, u32 offset, int y, int cb, int cr)
    {
        *reinterpret_cast<u16*>(mcu + offset) = ToRgb565(y, cb, cr);
    }
};

struct Rgb8Format : LinearShrinkLayout<3>
{
    static void Put(u8* mcu, u32 offset, int y, int cb, int cr)
    {
        mcu[offset] = Saturate(ToRed(y, cr));
        mcu[offset + 1] = Saturate(ToGreen(y, cb, cr));
        mcu[offset + 2] = Saturate(ToBlue(y, cb));
    }
};

struct Bgr8Format : LinearShrinkLayout<3>
{
    static void Put(u8* mcu, u32 offset, int y, int cb, int cr)
    {
        mcu[offset] = Saturate(ToBlue(y, cb));
        mcu[offset + 1] = Saturate(ToGreen(y, cb, cr));
        mcu[offset + 2] = Saturate(ToRed(y, cr));
    }
};

// the block format of the GPU: B, G, R in memory
struct Rgb8Block8Format : Block8Layout<3>
{
    static void Put(u8* mcu, u32 offset, int y, int cb, int cr)
    {
        mcu[offset] = Saturate(ToBlue(y, cb));
        mcu[offset + 1] = Saturate(ToGreen(y, cb, cr));
        mcu[offset + 2] = Saturate(ToRed(y, cr));
    }
};

struct Rgba8Format : LinearShrinkLayout<4>
{
    static void Put(u8* mcu, u32 offset, int y, int cb, int cr)
    {
        mcu[offset] = Saturate(ToRed(y, cr));
        mcu[offset + 1] = Saturate(ToGreen(y, cb, cr));
        mcu[offset + 2] = Saturate(ToBlue(y, cb));
        mcu[offset + 3] = 0xFF;
    }
};

struct Abgr8Format : LinearShrinkLayout<4>
{
    static void Put(u8* mcu, u32 offset, int y, int cb, int cr)
    {
        mcu[offset] = 0xFF;
        mcu[offset + 1] = Saturate(ToBlue(y, cb));
        mcu[offset + 2] = Saturate(ToGreen(y, cb, cr));
        mcu[offset + 3] = Saturate(ToRed(y, cr));
    }
};

// the block format of the GPU: A, B, G, R in memory
struct Rgba8Block8Format : Block8Layout<4>
{
    static void Put(u8* mcu, u32 offset, int y, int cb, int cr)
    {
        mcu[offset] = 0xFF;
        mcu[offset + 1] = Saturate(ToBlue(y, cb));
        mcu[offset + 2] = Saturate(ToGreen(y, cb, cr));
        mcu[offset + 3] = Saturate(ToRed(y, cr));
    }
};

// an MCU of H x V luma blocks (the chroma blocks cover the whole MCU), cut at the image edge
template <class Format, int H, int V>
inline void WriteMcu(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    u8* mcu = Format::GetMcu(context, mcuX, mcuY);
    for (int by = 0; by < V; by++) {
        for (int bx = 0; bx < H; bx++) {
            for (int y = 0; y < 8; y++) {
                const int py = by * 8 + y;
                if (mcuY + py >= context->m_Height) {
                    break;
                }
                for (int x = 0; x < 8; x++) {
                    const int px = bx * 8 + x;
                    if (mcuX + px >= context->m_Width) {
                        break;
                    }
                    const int chroma = (py / V) * 8 + px / H;
                    Format::Put(mcu, Format::GetOffset(context->m_Stride, px, py, bx, by, x, y), context->m_Y[by * H + bx][y * 8 + x],
                                context->m_Cb[chroma], context->m_Cr[chroma]);
                }
            }
        }
    }
}

// the same, scaled down by 1 << shrink: an MCU that is smaller than the step and not at a
// multiple of it is left out
template <class Format, int H, int V>
inline void ShrinkMcu(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    const int shrink = context->m_Shrink;
    const int step = 1 << shrink;
    const int sx = mcuX >> shrink;
    const int sy = mcuY >> shrink;
    if (V == 1 && (sy << shrink) != mcuY) {
        return;
    }
    if (H == 1 && (sx << shrink) != mcuX) {
        return;
    }
    u8* mcu = Format::GetShrunkMcu(context, sx, sy);
    for (int by = 0; by < V; by++) {
        for (int bx = 0; bx < H; bx++) {
            for (int y = 0; y < 8; y += step) {
                const int py = by * 8 + y;
                if (mcuY + py >= context->m_Height) {
                    break;
                }
                for (int x = 0; x < 8; x += step) {
                    const int px = bx * 8 + x;
                    if (mcuX + px >= context->m_Width) {
                        break;
                    }
                    const int chroma = (py / V) * 8 + px / H;
                    const int col = bx * (8 >> shrink) + (x >> shrink);
                    Format::Put(mcu, Format::GetShrunkOffset(context->m_Stride, sx, sy, col, py >> shrink),
                                context->m_Y[by * H + bx][y * 8 + x], context->m_Cb[chroma], context->m_Cr[chroma]);
                }
            }
        }
    }
}
} // namespace

// 0x00476294 (name is ours, after the writer tables)
void JpegMpDecoderCWrite11Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Bgr8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x00476398 (name is ours, after the writer tables)
void JpegMpDecoderCWrite11Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x0047649C (name is ours, after the writer tables)
void JpegMpDecoderCWrite12Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Bgr8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047660C (name is ours, after the writer tables)
void JpegMpDecoderCWrite12Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047677C (name is ours, after the writer tables)
void JpegMpDecoderCWrite21Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Bgr8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x004768F0 (name is ours, after the writer tables)
void JpegMpDecoderCWrite21Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x00476A64 (name is ours, after the writer tables)
void JpegMpDecoderCWrite22Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Bgr8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x00476C2C (name is ours, after the writer tables)
void JpegMpDecoderCWrite22Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x00476DF4 (name is ours, after the writer tables)
void JpegMpDecoderCShrink11Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Bgr8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x00476F58 (name is ours, after the writer tables)
void JpegMpDecoderCShrink11Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x004770BC (name is ours, after the writer tables)
void JpegMpDecoderCShrink12Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Bgr8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x00477254 (name is ours, after the writer tables)
void JpegMpDecoderCShrink12Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x004773EC (name is ours, after the writer tables)
void JpegMpDecoderCShrink21Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Bgr8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x004775A8 (name is ours, after the writer tables)
void JpegMpDecoderCShrink21Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x00477764 (name is ours, after the writer tables)
void JpegMpDecoderCShrink22Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Bgr8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x00477968 (name is ours, after the writer tables)
void JpegMpDecoderCShrink22Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x00477B6C (name is ours, after the writer tables)
void JpegMpDecoderCWrite11Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Abgr8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x00477C70 (name is ours, after the writer tables)
void JpegMpDecoderCWrite11Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgba8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x00477D74 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite11Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Yuyv8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x00477E34 (name is ours, after the writer tables)
void JpegMpDecoderCWrite12Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Abgr8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x00477FA4 (name is ours, after the writer tables)
void JpegMpDecoderCWrite12Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgba8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x00478110 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite12Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Yuyv8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047820C (name is ours, after the writer tables)
void JpegMpDecoderCWrite21Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Abgr8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x00478380 (name is ours, after the writer tables)
void JpegMpDecoderCWrite21Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgba8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x004784F4 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite21Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Yuyv8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x004785E8 (name is ours, after the writer tables)
void JpegMpDecoderCWrite22Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Abgr8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x004787B4 (name is ours, after the writer tables)
void JpegMpDecoderCWrite22Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgba8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x0047897C | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite22Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Yuyv8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x00478ACC (name is ours, after the writer tables)
void JpegMpDecoderCShrink11Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Abgr8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x00478C3C (name is ours, after the writer tables)
void JpegMpDecoderCShrink11Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgba8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x00478DAC | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink11Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Yuyv8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x00478EA4 (name is ours, after the writer tables)
void JpegMpDecoderCShrink12Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Abgr8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x00479058 (name is ours, after the writer tables)
void JpegMpDecoderCShrink12Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgba8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047920C | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink12Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Yuyv8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x00479350 (name is ours, after the writer tables)
void JpegMpDecoderCShrink21Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Abgr8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x0047950C (name is ours, after the writer tables)
void JpegMpDecoderCShrink21Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgba8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x004796C8 | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink21Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Yuyv8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x0047980C (name is ours, after the writer tables)
void JpegMpDecoderCShrink22Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Abgr8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x00479A10 (name is ours, after the writer tables)
void JpegMpDecoderCShrink22Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgba8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x00479C14 | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink22Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Yuyv8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x00479FD8 (name is ours, after the writer tables)
void JpegMpDecoderCWrite11CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb565Format, 1, 1>(context, mcuX, mcuY);
}

// 0x0047A0E0 (name is ours, after the writer tables)
void JpegMpDecoderCWrite12CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb565Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047A240 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite21CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb565Format, 2, 1>(context, mcuX, mcuY);
}

// 0x0047A3A4 (name is ours, after the writer tables)
void JpegMpDecoderCWrite22CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb565Format, 2, 2>(context, mcuX, mcuY);
}

// 0x0047A560 | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink11CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb565Format, 1, 1>(context, mcuX, mcuY);
}

// 0x0047A6C8 (name is ours, after the writer tables)
void JpegMpDecoderCShrink12CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb565Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047A868 | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink21CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb565Format, 2, 1>(context, mcuX, mcuY);
}

// 0x0047AA14 (name is ours, after the writer tables)
void JpegMpDecoderCShrink22CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb565Format, 2, 2>(context, mcuX, mcuY);
}

// 0x0047AF64 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite11CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb8Block8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x0047B0A0 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite12CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb8Block8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047B248 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite21CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb8Block8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x0047B3E0 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite22CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb8Block8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x0047B5FC | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink11CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb8Block8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x0047B7A4 | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink12CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb8Block8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047B9AC | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink21CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb8Block8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x0047BBC4 | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink22CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb8Block8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x0047BE20 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite11CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgba8Block8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x0047BF74 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite12CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgba8Block8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047C150 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite21CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgba8Block8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x0047C318 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite22CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgba8Block8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x0047C550 | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink11CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgba8Block8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x0047C70C | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink12CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgba8Block8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047C910 | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink21CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgba8Block8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x0047CB20 | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink22CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgba8Block8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x0047CDA8 (name is ours, after the writer tables)
void JpegMpDecoderCWrite11CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb565Block8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x0047CEBC (name is ours, after the writer tables)
void JpegMpDecoderCWrite12CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb565Block8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047D044 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite21CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb565Block8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x0047D1A0 | nintendogs:bytes [tier B]
void JpegMpDecoderCWrite22CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    WriteMcu<Rgb565Block8Format, 2, 2>(context, mcuX, mcuY);
}

// 0x0047D378 (name is ours, after the writer tables)
void JpegMpDecoderCShrink11CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb565Block8Format, 1, 1>(context, mcuX, mcuY);
}

// 0x0047D508 (name is ours, after the writer tables)
void JpegMpDecoderCShrink12CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb565Block8Format, 1, 2>(context, mcuX, mcuY);
}

// 0x0047D6D4 | nintendogs:bytes [tier B]
void JpegMpDecoderCShrink21CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb565Block8Format, 2, 1>(context, mcuX, mcuY);
}

// 0x0047D8AC (name is ours, after the writer tables)
void JpegMpDecoderCShrink22CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY)
{
    ShrinkMcu<Rgb565Block8Format, 2, 2>(context, mcuX, mcuY);
}
} // namespace detail
} // namespace CTR
} // namespace jpeg
} // namespace nn
