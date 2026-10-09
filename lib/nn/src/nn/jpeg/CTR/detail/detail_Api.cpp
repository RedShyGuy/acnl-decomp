// The decoder: the markers of the file, the tables and the scan (the blocks are decoded by the
// assembly, jpeg_Asm.cpp).

#include <string.h>
#include "nn/jpeg/CTR/detail/detail_Api.h"
#include "nn/jpeg/CTR/jpeg_JpegMpDecoder.h"

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail {
namespace {
const s8 ERROR_INVALID_PARAMETER = -2;
const s8 ERROR_ALIGNMENT = -3;
const s8 ERROR_SHORT_OF_BUFFER = -4;
const s8 ERROR_ASM = -10;
const s8 ERROR_TOO_LARGE = -20;
const s8 ERROR_UNEXPECTED_SIZE = -21;
const s8 ERROR_APP1 = -30;
const s8 ERROR_THUMBNAIL = -31;
const s8 ERROR_APP2 = -32;
const s8 ERROR_MISSING_SEGMENT = -50;
const s8 ERROR_NO_SOI = -60;
const s8 ERROR_SOF = -61;
const s8 ERROR_UNSUPPORTED_SOF = -62;
const s8 ERROR_DHT = -63;
const s8 ERROR_SOS = -64;
const s8 ERROR_DQT = -65;
const s8 ERROR_DRI = -66;
const s8 ERROR_NO_SCAN = -67;
const s8 ERROR_NO_SOF = -68;
const s8 ERROR_NO_DQT = -69;
const s8 ERROR_NO_DHT = -70;
const s8 ERROR_MARKER = -90;
const s8 ERROR_END = -91;
const s8 ERROR_DQT_SIZE = -96;
const s8 ERROR_INTERNAL = -127;

// JpegMpDecoderContext::m_Flags
const u32 FLAG_SOF = 0x1;
const u32 FLAG_DC_TABLE_0 = 0x10;
const u32 FLAG_DC_TABLE_1 = 0x20;
const u32 FLAG_AC_TABLE_0 = 0x40;
const u32 FLAG_AC_TABLE_1 = 0x80;
const u32 FLAG_HUFFMAN_TABLES = 0xF0;
const u32 FLAG_DQT = 0x1000;
const u32 FLAG_SCAN = 0x10000;

// JpegMpDecoderContext::m_SettingFlags
const u32 SETTING_DEFAULT_HUFFMAN_TABLES = 0x1;
const u32 SETTING_EXACT_SIZE = 0x2;

// the position of the coefficient of each quantization value (in the order of the file) in
// the order of the assembly
// 0x008A02C0
const u8 s_QuantizationOrder[64] = {
    4,  12, 2,  6,  10, 20, 28, 18, 14, 1,  5,  9,  22, 26, 36, 44, 34, 30, 17, 13, 0,  7,  8,  21, 25, 38, 42, 52, 60, 50, 46, 33,
    29, 16, 15, 3,  11, 23, 24, 37, 41, 54, 58, 62, 49, 45, 32, 31, 19, 27, 39, 40, 53, 57, 61, 48, 47, 35, 43, 55, 56, 63, 51, 59,
};

// the scale factors of the inverse DCT (AAN), 1.14 fixed point
// 0x008A0880
const s16 s_IdctScale[64] = {
    16384, 22725, 22725, 21407, 31521, 21407, 19266, 29692, 29692, 19266, 16384, 26722, 27969, 26722, 16384, 12873,
    22725, 25172, 25172, 22725, 12873, 8867,  17855, 21407, 22654, 21407, 17855, 8867,  4520,  12299, 16819, 19266,
    19266, 16819, 12299, 4520,  6270,  11585, 15137, 16384, 15137, 11585, 6270,  5906,  10426, 12873, 12873, 10426,
    5906,  5315,  8867,  10114, 8867,  5315,  4520,  6967,  6967,  4520,  3552,  4799,  3552,  2446,  2446,  1247,
};

// the writers by [format][vertical sampling - 1][horizontal sampling - 1]: the assembly for
// images of whole MCUs, the C writers for the rest and the C writers that scale down
// 0x008A0910
JpegMpDecoderWriteFunc const s_AsmWriteFuncs[PIXEL_FORMAT_COUNT][2][2] = {
    {{JpegMpDecoderAsmWrite11Yuyv8, JpegMpDecoderAsmWrite21Yuyv8}, {JpegMpDecoderAsmWrite12Yuyv8, JpegMpDecoderAsmWrite22Yuyv8}},
    {{JpegMpDecoderAsmWrite11CtrRgb565, JpegMpDecoderAsmWrite21CtrRgb565}, {JpegMpDecoderAsmWrite12CtrRgb565, JpegMpDecoderAsmWrite22CtrRgb565}},
    {{JpegMpDecoderAsmWrite11CtrRgb565Block8, JpegMpDecoderAsmWrite21CtrRgb565Block8},
     {JpegMpDecoderAsmWrite12CtrRgb565Block8, JpegMpDecoderAsmWrite22CtrRgb565Block8}},
    {{JpegMpDecoderAsmWrite11Rgb8, JpegMpDecoderAsmWrite21Rgb8}, {JpegMpDecoderAsmWrite12Rgb8, JpegMpDecoderAsmWrite22Rgb8}},
    {{JpegMpDecoderAsmWrite11CtrRgb8Block8, JpegMpDecoderAsmWrite21CtrRgb8Block8},
     {JpegMpDecoderAsmWrite12CtrRgb8Block8, JpegMpDecoderAsmWrite22CtrRgb8Block8}},
    {{JpegMpDecoderAsmWrite11Rgba8, JpegMpDecoderAsmWrite21Rgba8}, {JpegMpDecoderAsmWrite12Rgba8, JpegMpDecoderAsmWrite22Rgba8}},
    {{JpegMpDecoderAsmWrite11CtrRgba8Block8, JpegMpDecoderAsmWrite21CtrRgba8Block8},
     {JpegMpDecoderAsmWrite12CtrRgba8Block8, JpegMpDecoderAsmWrite22CtrRgba8Block8}},
    {{JpegMpDecoderAsmWrite11Bgr8, JpegMpDecoderAsmWrite21Bgr8}, {JpegMpDecoderAsmWrite12Bgr8, JpegMpDecoderAsmWrite22Bgr8}},
    {{JpegMpDecoderAsmWrite11Abgr8, JpegMpDecoderAsmWrite21Abgr8}, {JpegMpDecoderAsmWrite12Abgr8, JpegMpDecoderAsmWrite22Abgr8}},
};
// 0x008A09A0
JpegMpDecoderWriteFunc const s_CWriteFuncs[PIXEL_FORMAT_COUNT][2][2] = {
    {{JpegMpDecoderCWrite11Yuyv8, JpegMpDecoderCWrite21Yuyv8}, {JpegMpDecoderCWrite12Yuyv8, JpegMpDecoderCWrite22Yuyv8}},
    {{JpegMpDecoderCWrite11CtrRgb565, JpegMpDecoderCWrite21CtrRgb565}, {JpegMpDecoderCWrite12CtrRgb565, JpegMpDecoderCWrite22CtrRgb565}},
    {{JpegMpDecoderCWrite11CtrRgb565Block8, JpegMpDecoderCWrite21CtrRgb565Block8},
     {JpegMpDecoderCWrite12CtrRgb565Block8, JpegMpDecoderCWrite22CtrRgb565Block8}},
    {{JpegMpDecoderCWrite11Rgb8, JpegMpDecoderCWrite21Rgb8}, {JpegMpDecoderCWrite12Rgb8, JpegMpDecoderCWrite22Rgb8}},
    {{JpegMpDecoderCWrite11CtrRgb8Block8, JpegMpDecoderCWrite21CtrRgb8Block8},
     {JpegMpDecoderCWrite12CtrRgb8Block8, JpegMpDecoderCWrite22CtrRgb8Block8}},
    {{JpegMpDecoderCWrite11Rgba8, JpegMpDecoderCWrite21Rgba8}, {JpegMpDecoderCWrite12Rgba8, JpegMpDecoderCWrite22Rgba8}},
    {{JpegMpDecoderCWrite11CtrRgba8Block8, JpegMpDecoderCWrite21CtrRgba8Block8},
     {JpegMpDecoderCWrite12CtrRgba8Block8, JpegMpDecoderCWrite22CtrRgba8Block8}},
    {{JpegMpDecoderCWrite11Bgr8, JpegMpDecoderCWrite21Bgr8}, {JpegMpDecoderCWrite12Bgr8, JpegMpDecoderCWrite22Bgr8}},
    {{JpegMpDecoderCWrite11Abgr8, JpegMpDecoderCWrite21Abgr8}, {JpegMpDecoderCWrite12Abgr8, JpegMpDecoderCWrite22Abgr8}},
};
// 0x008A0A30
JpegMpDecoderWriteFunc const s_CShrinkFuncs[PIXEL_FORMAT_COUNT][2][2] = {
    {{JpegMpDecoderCShrink11Yuyv8, JpegMpDecoderCShrink21Yuyv8}, {JpegMpDecoderCShrink12Yuyv8, JpegMpDecoderCShrink22Yuyv8}},
    {{JpegMpDecoderCShrink11CtrRgb565, JpegMpDecoderCShrink21CtrRgb565}, {JpegMpDecoderCShrink12CtrRgb565, JpegMpDecoderCShrink22CtrRgb565}},
    {{JpegMpDecoderCShrink11CtrRgb565Block8, JpegMpDecoderCShrink21CtrRgb565Block8},
     {JpegMpDecoderCShrink12CtrRgb565Block8, JpegMpDecoderCShrink22CtrRgb565Block8}},
    {{JpegMpDecoderCShrink11Rgb8, JpegMpDecoderCShrink21Rgb8}, {JpegMpDecoderCShrink12Rgb8, JpegMpDecoderCShrink22Rgb8}},
    {{JpegMpDecoderCShrink11CtrRgb8Block8, JpegMpDecoderCShrink21CtrRgb8Block8},
     {JpegMpDecoderCShrink12CtrRgb8Block8, JpegMpDecoderCShrink22CtrRgb8Block8}},
    {{JpegMpDecoderCShrink11Rgba8, JpegMpDecoderCShrink21Rgba8}, {JpegMpDecoderCShrink12Rgba8, JpegMpDecoderCShrink22Rgba8}},
    {{JpegMpDecoderCShrink11CtrRgba8Block8, JpegMpDecoderCShrink21CtrRgba8Block8},
     {JpegMpDecoderCShrink12CtrRgba8Block8, JpegMpDecoderCShrink22CtrRgba8Block8}},
    {{JpegMpDecoderCShrink11Bgr8, JpegMpDecoderCShrink21Bgr8}, {JpegMpDecoderCShrink12Bgr8, JpegMpDecoderCShrink22Bgr8}},
    {{JpegMpDecoderCShrink11Abgr8, JpegMpDecoderCShrink21Abgr8}, {JpegMpDecoderCShrink12Abgr8, JpegMpDecoderCShrink22Abgr8}},
};

inline void SetError(JpegMpDecoderContext* context, s8 error)
{
    if (context->m_Error == 0) {
        context->m_Error = error;
    }
}

// the width and height of the output buffer for a format (the block formats need whole tiles)
inline u32 GetAlignedWidth(u32 width, u32 format)
{
    switch (format) {
    case PIXEL_FORMAT_YUYV8:
        return (width + 1) & ~1;
    case PIXEL_FORMAT_CTR_RGB565:
    case PIXEL_FORMAT_RGB8:
    case PIXEL_FORMAT_RGBA8:
    case PIXEL_FORMAT_BGR8:
    case PIXEL_FORMAT_ABGR8:
        return width;
    case PIXEL_FORMAT_CTR_RGB565_BLOCK8:
    case PIXEL_FORMAT_CTR_RGB8_BLOCK8:
    case PIXEL_FORMAT_CTR_RGBA8_BLOCK8:
        return (width + 7) & ~7;
    default:
        return 0;
    }
}

inline u32 GetAlignedHeight(u32 height, u32 format)
{
    switch (format) {
    case PIXEL_FORMAT_YUYV8:
    case PIXEL_FORMAT_CTR_RGB565:
    case PIXEL_FORMAT_RGB8:
    case PIXEL_FORMAT_RGBA8:
    case PIXEL_FORMAT_BGR8:
    case PIXEL_FORMAT_ABGR8:
        return height;
    case PIXEL_FORMAT_CTR_RGB565_BLOCK8:
    case PIXEL_FORMAT_CTR_RGB8_BLOCK8:
    case PIXEL_FORMAT_CTR_RGBA8_BLOCK8:
        return (height + 7) & ~7;
    default:
        return 0;
    }
}

inline u32 ReadU16Big(const u8* p)
{
    return (p[0] << 8) | p[1];
}

// SOF0/SOF1: the size and the sampling; chooses the writer
DECOMP_ALWAYS_INLINE bool DecodeFrameHeader(JpegMpDecoderContext* context, u32 position, u32 length, bool isInThumbnail)
{
    const u8* src = context->m_Src;
    if (position + 9 >= context->m_SrcSize) {
        SetError(context, ERROR_SOF);
        return false;
    }
    // only the thumbnail's frame is decoded
    if (context->m_IsThumbnail && !isInThumbnail) {
        SetError(context, ERROR_THUMBNAIL);
        return false;
    }
    const u32 height = ReadU16Big(src + position + 3);
    context->m_Height = height;
    const u32 width = ReadU16Big(src + position + 5);
    context->m_Width = width;
    const u32 sampling = src[position + 9];
    const u32 cbTable = src[position + 13];
    const u32 crTable = src[position + 16];
    context->m_Position = position + length;
    if (context->m_IsExifOnly) {
        return true;
    }
    if (sampling != 0x11 && sampling != 0x21 && sampling != 0x12 && sampling != 0x22) {
        SetError(context, ERROR_UNSUPPORTED_SOF);
        return false;
    }
    context->m_SamplingH = sampling >> 4;
    context->m_SamplingV = sampling & 0xF;
    context->m_McuWidth = (sampling >> 4) * 8;
    context->m_McuHeight = (sampling & 0xF) * 8;
    if (cbTable >= 3 || crTable >= 3) {
        SetError(context, ERROR_UNSUPPORTED_SOF);
        return false;
    }
    context->m_CbQuantizationTable = cbTable;
    context->m_CrQuantizationTable = crTable;
    if (width == 0 || height == 0 || width > context->m_MaxWidth || height > context->m_MaxHeight) {
        SetError(context, ERROR_TOO_LARGE);
        return false;
    }
    if ((context->m_ExpectedWidth != 0 && context->m_ExpectedWidth != width) ||
        (context->m_ExpectedHeight != 0 && context->m_ExpectedHeight != height)) {
        SetError(context, ERROR_UNEXPECTED_SIZE);
        return false;
    }
    // the output may be larger than set up (a shrunk image rounds up)
    const s32 shrink = context->m_Shrink;
    const s32 mask = (1 << shrink) - 1;
    const s32 shrunkWidth = static_cast<s32>(width + mask) >> shrink;
    const s32 shrunkHeight = static_cast<s32>(height + mask) >> shrink;
    const u32 format = context->m_PixelFormat;
    if (GetAlignedWidth(shrunkWidth, format) > context->m_Stride) {
        context->m_Stride = GetAlignedWidth(shrunkWidth, format);
    }
    if (GetAlignedHeight(shrunkHeight, format) > context->m_AlignedHeight) {
        context->m_AlignedHeight = GetAlignedHeight(shrunkHeight, format);
    }
    if (JpegMpDecoder::GetDstBufferSize(context->m_Stride, context->m_AlignedHeight, static_cast<PixelFormat>(format)) > context->m_DstSize) {
        SetError(context, ERROR_INTERNAL);
        return false;
    }
    const u32 h = context->m_SamplingH;
    const u32 v = context->m_SamplingV;
    if (h == 0 || h > 2 || v == 0 || v > 2) {
        SetError(context, ERROR_UNSUPPORTED_SOF);
        return false;
    }
    context->m_WriteFunc = NULL;
    context->m_BlocksPerMcu = h * v;
    // the assembly writes whole MCUs only
    if ((context->m_Width & (context->m_McuWidth - 1)) == 0 && (context->m_Height & (context->m_McuHeight - 1)) == 0) {
        context->m_WriteFunc = s_AsmWriteFuncs[context->m_PixelFormat][v - 1][h - 1];
    }
    if (context->m_WriteFunc == NULL) {
        context->m_WriteFunc = s_CWriteFuncs[context->m_PixelFormat][v - 1][h - 1];
        if (context->m_WriteFunc == NULL) {
            SetError(context, ERROR_INTERNAL);
            return false;
        }
    }
    if (context->m_Shrink != 0) {
        context->m_WriteFunc = s_CShrinkFuncs[context->m_PixelFormat][v - 1][h - 1];
        if (context->m_WriteFunc == NULL) {
            SetError(context, ERROR_INTERNAL);
            return false;
        }
    }
    context->m_Flags |= FLAG_SOF;
    context->m_RequiredFlags |= FLAG_SCAN;
    return true;
}

// DHT: builds the decoding tables like the example of JPEG Annex C/F
DECOMP_ALWAYS_INLINE bool DecodeHuffmanTables(JpegMpDecoderContext* context, u32 position, u32 length)
{
    const u8* src = context->m_Src;
    if (position + 2 >= context->m_SrcSize) {
        SetError(context, ERROR_DHT);
        return false;
    }
    u32 p = position + 2;
    context->m_Position = position + length;
    while (p < context->m_Position) {
        if (p + 17 >= context->m_SrcSize) {
            SetError(context, ERROR_DHT);
            return false;
        }
        const u32 tableClass = src[p];   // class << 4 | id
        if (tableClass & 0xEE) {
            SetError(context, ERROR_DHT);
            return false;
        }
        u32 specOffset = 0;
        u32 decodeOffset = 0;
        if (tableClass & 1) {
            decodeOffset = 2 * sizeof(JpegMpDecoderHuffmanTable);
            specOffset = HUFFMAN_SPEC_SIZE;
        }
        if (tableClass & 0x10) {
            specOffset += HUFFMAN_SPEC_AC_OFFSET;
            decodeOffset += sizeof(JpegMpDecoderHuffmanTable);
        }
        u8* bits = context->m_HuffmanTables + specOffset;
        u8* values = bits + HUFFMAN_SPEC_VALUES_OFFSET;
        JpegMpDecoderHuffmanTable* table =
            reinterpret_cast<JpegMpDecoderHuffmanTable*>(context->m_HuffmanTables + HUFFMAN_DECODE_TABLES_OFFSET + decodeOffset);

        bits[0] = 0;
        s32 count = 0;
        for (s32 i = 0; i < 16; i++) {
            bits[i + 1] = src[p + 1 + i];
            count += src[p + 1 + i];
        }
        p += 17;
        if (p + count >= context->m_SrcSize) {
            SetError(context, ERROR_DHT);
            return false;
        }
        for (s32 i = 0; i < count; i++) {
            values[i] = src[p + i];
        }
        p += count;

        // the code lengths
        s32 k = 0;
        for (s32 l = 1; l <= 16; l++) {
            for (s32 i = 1; i <= bits[l] && k <= 256; i++) {
                context->m_HuffmanSize[k++] = l;
            }
        }
        if (k > 256) {
            SetError(context, ERROR_DHT);
            return false;
        }
        context->m_HuffmanSize[k] = 0;

        // the codes
        u32 code = 0;
        s32 size = context->m_HuffmanSize[0];
        k = 0;
        while (context->m_HuffmanSize[k] != 0) {
            while (context->m_HuffmanSize[k] == size) {
                context->m_HuffmanCode[k] = code;
                k++;
                if (k > 256) {
                    SetError(context, ERROR_DHT);
                    return false;
                }
                code++;
            }
            code <<= 1;
            size++;
        }

        // the decoding table
        k = 0;
        for (s32 l = 1; l < 17; l++) {
            if (bits[l] == 0) {
                table->m_MaxCode[l] = -1;
            } else {
                table->m_ValuePointer[l] = k;
                table->m_MinCode[l] = context->m_HuffmanCode[k];
                k += bits[l];
                if (k > 256) {
                    SetError(context, ERROR_DHT);
                    return false;
                }
                table->m_MaxCode[l] = context->m_HuffmanCode[k - 1];
            }
        }
        table->m_MaxCode[17] = 0xFFFFF;

        // the lookup of the codes of up to 8 bits
        k = 0;
        for (s32 l = 1; l <= 8; l++) {
            for (s32 i = 1; i <= bits[l]; i++, k++) {
                const s32 shift = 8 - l;
                const s32 n = 1 << shift;
                if (k > 256) {
                    SetError(context, ERROR_DHT);
                    return false;
                }
                s32 lookbits = context->m_HuffmanCode[k] << shift;
                // (the range of the last table's m_LookSymbol inside m_HuffmanTables)
                if (lookbits < -3118 || lookbits + n >= 3027) {
                    SetError(context, ERROR_DHT);
                    return false;
                }
                for (s32 j = n; j > 0; j--) {
                    const s32 index = HUFFMAN_DECODE_TABLES_OFFSET + decodeOffset + offsetof(JpegMpDecoderHuffmanTable, m_LookNbits) + lookbits;
                    if (index < 0 || index + 256 >= static_cast<s32>(sizeof(context->m_HuffmanTables))) {
                        SetError(context, ERROR_DHT);
                        return false;
                    }
                    table->m_LookNbits[lookbits] = l;
                    table->m_LookSymbol[lookbits] = values[k];
                    lookbits++;
                }
            }
        }

        u32 flag;
        switch (tableClass) {
        case 0x00:
            flag = FLAG_DC_TABLE_0;
            break;
        case 0x01:
            flag = FLAG_DC_TABLE_1;
            break;
        case 0x10:
            flag = FLAG_AC_TABLE_0;
            break;
        case 0x11:
            flag = FLAG_AC_TABLE_1;
            break;
        default:
            SetError(context, ERROR_INTERNAL);
            return false;
        }
        context->m_Flags |= flag;
    }
    return true;
}

// DQT: the quantization tables, scaled for the inverse DCT
DECOMP_ALWAYS_INLINE bool DecodeQuantizationTables(JpegMpDecoderContext* context, u32 position, u32 length)
{
    const u8* src = context->m_Src;
    u32 p = position + 2;
    context->m_Position = position + length;
    while (context->m_Position > p) {
        const u32 precisionAndId = src[p];
        const u32 id = precisionAndId & 0xF;
        if (id >= 3) {
            SetError(context, ERROR_DQT);
            return false;
        }
        p++;
        if (precisionAndId & 0xF0) {
            // 16 bit values
            if (context->m_SrcSize <= p + 128) {
                SetError(context, ERROR_DQT_SIZE);
                return false;
            }
            for (s32 i = 0; i < 64; i++) {
                const s32 value = (src[p] << 8) + src[p + 1];
                p += 2;
                context->m_Quantization[id][s_QuantizationOrder[i]] = ((value * s_IdctScale[i]) * 32 + 0x8000) >> 16;
            }
        } else {
            if (context->m_SrcSize <= p + 64) {
                SetError(context, ERROR_DQT_SIZE);
                return false;
            }
            for (s32 i = 0; i < 64; i++) {
                context->m_Quantization[id][s_QuantizationOrder[i]] = (src[p] * s_IdctScale[i] + 0x400) >> 11;
                p++;
            }
        }
    }
    context->m_Flags |= FLAG_DQT;
    return true;
}

// SOS: checks the scan header and decodes the image
DECOMP_ALWAYS_INLINE bool DecodeScan(JpegMpDecoderContext* context, u32 length)
{
    const u32 flags = context->m_Flags;
    if (flags & FLAG_SCAN) {
        SetError(context, ERROR_SOS);
        return false;
    }
    if (!(flags & FLAG_SOF)) {
        SetError(context, ERROR_NO_SOF);
        return false;
    }
    if (!(flags & FLAG_DQT)) {
        SetError(context, ERROR_NO_DQT);
        return false;
    }
    if (FLAG_HUFFMAN_TABLES & ~flags) {
        if (!(context->m_SettingFlags & SETTING_DEFAULT_HUFFMAN_TABLES)) {
            SetError(context, ERROR_NO_DHT);
            return false;
        }
        memcpy(context->m_HuffmanTables, s_DefaultHuffmanTables, sizeof(context->m_HuffmanTables));
    }
    const u32 position = context->m_Position;
    if (position + 12 >= context->m_SrcSize) {
        SetError(context, ERROR_SOS);
        return false;
    }
    // three components, baseline
    const u8* header = context->m_Src + position;
    if (length != 12 || header[2] != 3 || ((header[4] | header[6] | header[8]) & 0xEE) != 0 || header[9] != 0 || header[10] != 63 ||
        header[11] != 0) {
        SetError(context, ERROR_SOS);
        return false;
    }
    context->m_Position = position + 12;
    JpegMpDecoderAsmConvertWorkToAsm(context);
    if (context->m_WriteFunc == NULL) {
        SetError(context, ERROR_INTERNAL);
        return false;
    }
    for (u32 y = 0; y < context->m_Height; y += context->m_McuHeight) {
        for (u32 x = 0; x < context->m_Width; x += context->m_McuWidth) {
            if (context->m_RestartInterval != 0 && --context->m_RestartCounter == 0) {
                // skip the RST marker (the bits read ahead are given back)
                context->m_RestartCounter = context->m_RestartInterval;
                JpegMpDecoderAsmConvertWorkToC(context);
                if (context->m_BitCount > 7) {
                    context->m_Position = context->m_Position - (context->m_LastByte == 0xFF ? 1 : 0) - 1;
                }
                context->m_BitCount = 0;
                context->m_DcPrediction[2] = 0;
                context->m_DcPrediction[1] = 0;
                context->m_DcPrediction[0] = 0;
                context->m_Position += 2;
                JpegMpDecoderAsmConvertWorkToAsm(context);
            }
            for (u32 i = 0; i < context->m_BlocksPerMcu; i++) {
                JpegMpDecoderAsmGetMatrix(context, 0, &context->m_DcPrediction[0]);
                JpegMpDecoderAsmDecodeBlock(context, context->m_Y[i], 0);
            }
            JpegMpDecoderAsmGetMatrix(context, 2, &context->m_DcPrediction[1]);
            JpegMpDecoderAsmDecodeBlock(context, context->m_Cb, context->m_CbQuantizationTable);
            JpegMpDecoderAsmGetMatrix(context, 2, &context->m_DcPrediction[2]);
            JpegMpDecoderAsmDecodeBlock(context, context->m_Cr, context->m_CrQuantizationTable);
            if (context->m_Error != 0 || context->m_AsmError != 0) {
                JpegMpDecoderAsmConvertWorkToC(context);
                if (context->m_AsmError != 0) {
                    SetError(context, ERROR_ASM);
                }
                return false;
            }
            context->m_WriteFunc(context, x, y);
        }
    }
    JpegMpDecoderAsmConvertWorkToC(context);
    context->m_Flags |= FLAG_SCAN;
    return true;
}
} // namespace

// 0x00474540 | nintendogs:bytes-fuzzy [tier B]
void Mel_JPEGDecodeFast(JpegMpDecoderContext* context)
{
    // decoding the thumbnail: the frame of the JPEG inside the Exif data
    bool isInThumbnail = false;
    const u8* src = context->m_Src;
    const u32 start = context->m_Position;
    if (start + 2 >= context->m_SrcSize) {
        SetError(context, ERROR_NO_SOI);
        return;
    }
    if (src[start] != 0xFF || src[start + 1] != 0xD8) {
        SetError(context, ERROR_NO_SOI);
        return;
    }
    context->m_Position = start + 2;
restart:
    context->m_RestartCounter = 0;
    context->m_RestartInterval = 0;
    while (context->m_Position + 1 < context->m_SrcSize) {
        if (src[context->m_Position++] != 0xFF) {
            continue;
        }
        const u32 marker = src[context->m_Position++];
        if (marker == 0xD9) {
            break;
        }
        const u32 position = context->m_Position;
        if (position + 1 >= context->m_SrcSize) {
            SetError(context, ERROR_MARKER);
            return;
        }
        const u32 length = ReadU16Big(src + position);
        if (isInThumbnail) {
            switch (marker) {
            case 0xDB:
                if (!DecodeQuantizationTables(context, position, length)) {
                    return;
                }
                continue;
            case 0xDD:
                goto dri;
            case 0xC4:
                if (!DecodeHuffmanTables(context, position, length)) {
                    return;
                }
                continue;
            case 0xC0:
            case 0xC1:
                if (!DecodeFrameHeader(context, position, length, isInThumbnail)) {
                    return;
                }
                continue;
            case 0xDA:
                goto sos;
            default:
                break;
            }
        } else {
            switch (marker) {
            case 0xE1:
                if (!DecodeJpegApp1(context, length)) {
                    SetError(context, ERROR_APP1);
                    return;
                }
                continue;
            case 0xE2:
                if (!DecodeJpegApp2Mp(context, length)) {
                    SetError(context, ERROR_APP2);
                    return;
                }
                context->m_Position += length;
                continue;
            case 0xC0:
            case 0xC1:
                if (!DecodeFrameHeader(context, position, length, isInThumbnail)) {
                    return;
                }
                continue;
            case 0xC4:
                if (!DecodeHuffmanTables(context, position, length)) {
                    return;
                }
                continue;
            case 0xD8:
                // the SOI of the thumbnail: its tables are read anew
                if (context->m_IsThumbnail) {
                    isInThumbnail = true;
                    context->m_Flags = 0;
                    context->m_RequiredFlags = 0;
                    goto restart;
                }
                continue;
            case 0xDA:
                goto sos;
            case 0xDB:
                if (!DecodeQuantizationTables(context, position, length)) {
                    return;
                }
                continue;
            case 0xDD:
                goto dri;
            default:
                break;
            }
        }
        // other segments are skipped
        context->m_Position = position + length;
        continue;

    dri:
        if (position + 3 >= context->m_SrcSize) {
            SetError(context, ERROR_DRI);
            return;
        }
        context->m_RestartInterval = ReadU16Big(src + position + 2);
        context->m_RestartCounter = context->m_RestartInterval + 1;
        context->m_Position = position + length;
        continue;

    sos:
        if (context->m_IsExifOnly) {
            // the Exif data is read
            if (context->m_IsThumbnail && !isInThumbnail) {
                SetError(context, ERROR_THUMBNAIL);
            }
            return;
        }
        if (!DecodeScan(context, length)) {
            return;
        }
        break;
    }
    if (context->m_Position > context->m_SrcSize) {
        SetError(context, ERROR_END);
        return;
    }
    if (!(context->m_Flags & FLAG_SCAN)) {
        SetError(context, ERROR_NO_SCAN);
        return;
    }
    if (context->m_RequiredFlags & ~context->m_Flags) {
        SetError(context, ERROR_MISSING_SEGMENT);
    }
}

// 0x00479DA0 | nintendogs:bytes-fuzzy [tier B]
bool InitializeJpegMpDecoderContext(JpegMpDecoderContext* context, void* dst, size_t dstSize, const u8* src, size_t srcSize, u32 maxWidth,
                                    u32 maxHeight, PixelFormat format, bool isThumbnail, bool isExifOnly,
                                    const JpegMpDecoderTemporarySettingObj* settings)
{
    memset(context, 0, sizeof(JpegMpDecoderContext));
    if (!isExifOnly) {
        if (maxWidth == 0 || maxHeight == 0 || maxWidth >= 0x10000 || maxHeight >= 0x10000) {
            SetError(context, ERROR_INVALID_PARAMETER);
            return false;
        }
        if (reinterpret_cast<uptr>(dst) & 3) {
            SetError(context, ERROR_ALIGNMENT);
            return false;
        }
        u32 stride = settings->m_Stride;
        switch (format) {
        case PIXEL_FORMAT_YUYV8:
            stride &= ~1;
            break;
        case PIXEL_FORMAT_CTR_RGB565:
        case PIXEL_FORMAT_RGB8:
        case PIXEL_FORMAT_RGBA8:
        case PIXEL_FORMAT_BGR8:
        case PIXEL_FORMAT_ABGR8:
            break;
        case PIXEL_FORMAT_CTR_RGB565_BLOCK8:
        case PIXEL_FORMAT_CTR_RGB8_BLOCK8:
        case PIXEL_FORMAT_CTR_RGBA8_BLOCK8:
            stride &= ~7;
            break;
        default:
            SetError(context, ERROR_INVALID_PARAMETER);
            return false;
        }
        context->m_PixelFormat = format;
        context->m_SettingFlags = settings->m_Flags;
        if (settings->m_Flags & SETTING_EXACT_SIZE) {
            context->m_ExpectedWidth = maxWidth;
            context->m_ExpectedHeight = maxHeight;
        }
        context->m_Stride = stride;
        if (stride < maxWidth) {
            stride = maxWidth;
        }
        const size_t size = JpegMpDecoder::GetDstBufferSize(GetAlignedWidth(stride, format), GetAlignedHeight(maxHeight, format), format);
        if (size == 0 || size > dstSize) {
            SetError(context, ERROR_SHORT_OF_BUFFER);
            return false;
        }
    }
    if (srcSize == 0) {
        SetError(context, ERROR_NO_SOI);
        return false;
    }
    context->m_Src = src;
    context->m_SrcLast = src + srcSize - 1;
    context->m_SrcSize = srcSize;
    context->m_Dst = dst;
    context->m_DstSize = dstSize;
    context->m_MaxWidth = maxWidth;
    context->m_MaxHeight = maxHeight;
    context->m_IsThumbnail = isThumbnail;
    context->m_IsExifOnly = isExifOnly;
    return true;
}
} // namespace detail
} // namespace CTR
} // namespace jpeg
} // namespace nn
