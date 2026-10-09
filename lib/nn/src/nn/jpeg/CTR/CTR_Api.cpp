#include "nn/jpeg/CTR/CTR_Api.h"
#include <string.h>
#include "nn/jpeg/CTR/detail/detail_Api.h"
#include "nn/jpeg/CTR/detail/jpeg_EncoderWork.h"

// The baseline JPEG encoder (Yos_JPEGEncodeFast and the functions around it). The per-pixel work
// is done by the assembly functions in jpeg_Asm.cpp.

namespace nn {
namespace jpeg {
namespace CTR {
using detail::JpegMpEncoderTemporarySettingObj;
using detail::JpegMpEncoderWorkObj;

namespace {
// error codes of JpegMpEncoderContext::m_Error (names are ours)
const s8 ERROR_INVALID_PARAMETER = -2;
const s8 ERROR_MISALIGNED = -3;
const s8 ERROR_SHORT_OF_BUFFER = -9;
const s8 ERROR_UNSUPPORTED = -127;

const u32 HUFFMAN_TABLE_SEGMENT_SIZE = 420;
const u32 THUMBNAIL_QUALITY_MAX = 70;

// SOF0 (sizes and sampling are filled in) and the DQT marker
// 0x008A20A0
const u8 s_FrameHeader[23] = {
    0xFF, 0xC0, 0x00, 0x11, 0x08, 0x01, 0xE0, 0x02, 0x80, 0x03, 0x01, 0x11,
    0x00, 0x02, 0x11, 0x01, 0x03, 0x11, 0x01, 0xFF, 0xDB, 0x00, 0x84,
};

// SOS of the three components
// 0x008A20C0
const u8 s_ScanHeader[14] = { 0xFF, 0xDA, 0x00, 0x0C, 0x03, 0x01, 0x00, 0x02, 0x11, 0x03, 0x11, 0x00, 0x3F, 0x00 };

// the AAN scale factors of the forward DCT (multiplied with 512), divided by the quantization
// 0x008A20D0
const u32 s_AanScale[64] = {
    512, 369, 392, 435, 512, 652, 946, 1856, 369, 266, 283, 314, 369, 470, 682, 1338,
    392, 283, 300, 333, 392, 499, 724, 1420, 435, 314, 333, 370, 435, 554, 805, 1578,
    512, 369, 392, 435, 512, 652, 946, 1856, 652, 470, 499, 554, 652, 829, 1204, 2362,
    946, 682, 724, 805, 946, 1204, 1748, 3429, 1856, 1338, 1420, 1578, 1856, 2362, 3429, 6726,
};

// the quantization tables of the JPEG standard (Annex K), for quality 50
// 0x008A21D0
const u8 s_LumaQuantization[64] = {
    16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77, 24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101, 72, 92, 95, 98, 112, 100, 103, 99,
};

// 0x008A2210
const u8 s_ChromaQuantization[64] = {
    17, 18, 24, 47, 99, 99, 99, 99, 18, 21, 26, 66, 99, 99, 99, 99,
    24, 26, 56, 99, 99, 99, 99, 99, 47, 66, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
    99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
};

// 0x008A2250
const u8 s_ZigZag[64] = {
    0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5,
    12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6, 7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63,
};

// the converters of one MCU, by pixel format and sampling (4:4:4, 4:2:0, 4:2:2)
// 0x008A2290
JpegMpEncoderMcuFunc const s_ConvertFuncs[PIXEL_FORMAT_COUNT][3] = {
    { JpegMpEncoderAsm_Yuyv8ToYuv444, JpegMpEncoderAsm_Yuyv8ToYuv420, JpegMpEncoderAsm_Yuyv8ToYuv422 },
    { JpegMpEncoderAsm_CtrRgb565ToYuv444, JpegMpEncoderAsm_CtrRgb565ToYuv420, JpegMpEncoderAsm_CtrRgb565ToYuv422 },
    { JpegMpEncoderAsm_CtrRgb565Block8ToYuv444, JpegMpEncoderAsm_CtrRgb565Block8ToYuv420, JpegMpEncoderAsm_CtrRgb565Block8ToYuv422 },
    { JpegMpEncoderAsm_Rgb8ToYuv444, JpegMpEncoderAsm_Rgb8ToYuv420, JpegMpEncoderAsm_Rgb8ToYuv422 },
    { JpegMpEncoderAsm_CtrRgb8Block8ToYuv444, JpegMpEncoderAsm_CtrRgb8Block8ToYuv420, JpegMpEncoderAsm_CtrRgb8Block8ToYuv422 },
    { JpegMpEncoderAsm_Rgba8ToYuv444, JpegMpEncoderAsm_Rgba8ToYuv420, JpegMpEncoderAsm_Rgba8ToYuv422 },
    { JpegMpEncoderAsm_CtrRgba8Block8ToYuv444, JpegMpEncoderAsm_CtrRgba8Block8ToYuv420, JpegMpEncoderAsm_CtrRgba8Block8ToYuv422 },
    { JpegMpEncoderAsm_Bgr8ToYuv444, JpegMpEncoderAsm_Bgr8ToYuv420, JpegMpEncoderAsm_Bgr8ToYuv422 },
    { JpegMpEncoderAsm_Abgr8ToYuv444, JpegMpEncoderAsm_Abgr8ToYuv420, JpegMpEncoderAsm_Abgr8ToYuv422 },
};

// the thumbnail resamplers, by pixel format
// 0x008A22FC
JpegMpEncoderMcuFunc const s_ResampleFuncs[PIXEL_FORMAT_COUNT] = {
    JpegMpEncoderCResampleYuyv8,
    JpegMpEncoderCResampleCtrRgb565,
    JpegMpEncoderCResampleCtrRgb565Block8,
    JpegMpEncoderCResampleRgb8,
    JpegMpEncoderCResampleCtrRgb8Block8,
    JpegMpEncoderCResampleRgba8,
    JpegMpEncoderCResampleCtrRgba8Block8,
    JpegMpEncoderCResampleBgr8,
    JpegMpEncoderCResampleAbgr8,
};

// 0x008A2320
const u32 s_BitMask[17] = {
    0x0, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
};

// the position of a pixel (y * 8 + x) in an 8x8 tile of the Block8 formats
// 0x008A2380
const u8 s_Block8Swizzle[64] = {
    0, 1, 4, 5, 16, 17, 20, 21, 2, 3, 6, 7, 18, 19, 22, 23,
    8, 9, 12, 13, 24, 25, 28, 29, 10, 11, 14, 15, 26, 27, 30, 31,
    32, 33, 36, 37, 48, 49, 52, 53, 34, 35, 38, 39, 50, 51, 54, 55,
    40, 41, 44, 45, 56, 57, 60, 61, 42, 43, 46, 47, 58, 59, 62, 63,
};

inline void SetError(JpegMpEncoderContext* context, s8 error)
{
    if (context->m_Error == 0) {
        context->m_Error = error;
    }
}

// writes the full byte of the bit buffer (with a stuffed 0 behind 0xFF)
inline bool FlushBitBuffer(JpegMpEncoderContext* context)
{
    context->m_Dst[context->m_DstPosition] = context->m_BitBuffer;
    if (context->m_BitBuffer == 0xFF) {
        if (context->m_DstPosition + 2 >= context->m_DstSize) {
            SetError(context, ERROR_SHORT_OF_BUFFER);
            return false;
        }
        context->m_Dst[context->m_DstPosition + 1] = 0;
        context->m_DstPosition += 2;
    } else {
        context->m_DstPosition += 1;
        if (context->m_DstSize <= context->m_DstPosition) {
            SetError(context, ERROR_SHORT_OF_BUFFER);
            return false;
        }
    }
    context->m_BitBuffer = 0;
    context->m_BitsLeft = 8;
    return true;
}

inline bool PutBits(JpegMpEncoderContext* context, u32 value, s32 count)
{
    for (s32 i = count; i != 0; --i) {
        if (value & s_BitMask[i]) {
            context->m_BitBuffer |= s_BitMask[context->m_BitsLeft];
        }
        if (--context->m_BitsLeft == 0 && !FlushBitBuffer(context)) {
            return false;
        }
    }
    return true;
}

// the source position of a resampled MCU; the converter indexes the buffer by absolute position
inline void FinishResample(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY)
{
    context->m_Source = context->m_ResampleBuffer - (mcuY * context->m_SamplingV * 128 + mcuX * context->m_SamplingH * 8);
    context->m_ConvertFunc(context, mcuX, mcuY);
}

// nearest-neighbor resampling of one MCU into CtrRgb565 (right to left, bottom to top)
template <typename Reader>
inline void ResampleToRgb565(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY, Reader read)
{
    const s32 mcuWidth = context->m_SamplingH * 8;
    u32 row = context->m_SamplingV * 8;
    u32 y = (mcuY + 1) * row;
    const u32 sourceWidth = context->m_ResampleSourceWidth;
    const u32 width = context->m_Width;
    const u32 stepX = (sourceWidth << 4) / width;
    const u32 startX = ((mcuX + 1) * mcuWidth - width) * stepX + (sourceWidth << 4);
    while (row != 0) {
        --y;
        --row;
        const u32 sourceY = ((context->m_ResampleSourceHeight * y) << 4) / context->m_Height >> 4;
        u32 sourceX = startX;
        for (s32 x = mcuWidth - 1; x >= 0; --x) {
            sourceX -= stepX;
            context->m_ResampleBuffer[row * 16 + x] = read(context, sourceY, sourceX >> 4);
        }
    }
    FinishResample(context, mcuX, mcuY);
}

inline u16 ToRgb565(u8 r, u8 g, u8 b)
{
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

// the pixel index in a Block8 image (tiles of 8x8 pixels, swizzled inside)
inline u32 GetBlock8Index(const JpegMpEncoderContext* context, u32 x, u32 y)
{
    return s_Block8Swizzle[(x & 7) | ((y & 7) << 3)] + (y & ~7) * context->m_ResampleSourceStride + (x & ~7) * 8;
}
} // namespace

// 0x00470BC4 | nintendogs:bytes [tier B]
bool StartMpEncoderCore(JpegMpEncoderWorkObj* work, u8* dst, u32 dstSize, const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, bool isAddThumbnail, JpegMpEncoderTemporarySettingObj* settings)
{
    if (settings->m_NintendoData != NULL) {
        SetError(&work->m_Context, ERROR_UNSUPPORTED);
        return false;
    }
    work->m_JpegStart = dst;
    work->m_IsMp = true;
    const u32 size = StartJpegEncoderCore(work, dst, dstSize, src, width, height, quality, sampling, format, isAddThumbnail, settings);
    if (size == 0) {
        return false;
    }
    work->m_JpegSize = size;
    if (!detail::EncodeJpegMpApp2(work, settings)) {
        return false;
    }
    work->m_ImageStart = reinterpret_cast<uptr>(work->m_Context.m_Dst) + work->m_Context.m_DstPosition;
    if (Yos_JPEGEncodeFast(&work->m_Context) == 0) {
        return false;
    }
    if (!detail::PostEncodeJpegMpApp2(work, false)) {
        return false;
    }
    work->m_ImageSize = reinterpret_cast<uptr>(work->m_Context.m_Dst) + work->m_Context.m_DstPosition - work->m_ImageStart;
    return true;
}

// 0x00470CC8
u32 Yos_JPEGEncodeFast(JpegMpEncoderContext* context)
{
    const u32 huffmanTableSize = (context->m_IsNoHuffmanTable == 0) ? HUFFMAN_TABLE_SEGMENT_SIZE : 0;
    u8* p = context->m_Dst + context->m_DstPosition;
    u8* tables = p + huffmanTableSize;
    if (context->m_Dst + context->m_DstSize <= tables + 167) {
        SetError(context, ERROR_SHORT_OF_BUFFER);
        return 0;
    }

    // SOF0 and DQT
    memcpy(p, s_FrameHeader, sizeof(s_FrameHeader));
    p[5] = context->m_Height >> 8;
    p[6] = context->m_Height;
    p[7] = context->m_Width >> 8;
    p[8] = context->m_Width;
    p[11] = (context->m_SamplingH << 4) | context->m_SamplingV;
    p[23] = 0;
    for (s32 i = 0; i < 64; ++i) {
        p[24 + i] = context->m_LumaQuantization[s_ZigZag[i]];
    }
    p[88] = 1;
    for (s32 i = 0; i < 64; ++i) {
        p[89 + i] = context->m_ChromaQuantization[s_ZigZag[i]];
    }
    p += 153;
    if (huffmanTableSize != 0) {
        memcpy(p, detail::s_HuffmanTableSegment, HUFFMAN_TABLE_SEGMENT_SIZE);
        p += HUFFMAN_TABLE_SEGMENT_SIZE;
    }
    memcpy(p, s_ScanHeader, sizeof(s_ScanHeader));
    context->m_DstPosition = p + sizeof(s_ScanHeader) - context->m_Dst;

    context->m_BitBuffer = 0;
    context->m_BitsLeft = 8;
    context->m_Y.m_DcPrediction = 0;
    context->m_Cb.m_DcPrediction = 0;
    context->m_Cr.m_DcPrediction = 0;
    JpegMpEncoderAsm_ConvertWorkToAsm(context);
    for (u32 y = 0; y < context->m_McuCountY; ++y) {
        for (u32 x = 0; x < context->m_McuCountX; ++x) {
            context->m_McuFunc(context, x, y);
            for (u32 i = 0; i < context->m_BlocksPerMcu; ++i) {
                JpegMpEncoderAsm_ForwardDct(context->m_YBlocks[i], reinterpret_cast<const s16*>(context->m_LumaReciprocal), context->m_Y.m_Coefficients);
                if (!JpegMpEncoderAsm_ComponentSequential(context, &context->m_Y)) {
                    SetError(context, ERROR_SHORT_OF_BUFFER);
                    return 0;
                }
            }
            JpegMpEncoderAsm_ForwardDct(context->m_CbBlock, reinterpret_cast<const s16*>(context->m_ChromaReciprocal), context->m_Cb.m_Coefficients);
            if (!JpegMpEncoderAsm_ComponentSequential(context, &context->m_Cb)) {
                SetError(context, ERROR_SHORT_OF_BUFFER);
                return 0;
            }
            JpegMpEncoderAsm_ForwardDct(context->m_CrBlock, reinterpret_cast<const s16*>(context->m_ChromaReciprocal), context->m_Cr.m_Coefficients);
            if (!JpegMpEncoderAsm_ComponentSequential(context, &context->m_Cr)) {
                SetError(context, ERROR_SHORT_OF_BUFFER);
                return 0;
            }
        }
    }
    JpegMpEncoderAsm_ConvertWorkToC(context);

    // fill up the last byte (with 0 bits)
    if (context->m_BitsLeft != 8 && !PutBits(context, 0, 7)) {
        SetError(context, ERROR_SHORT_OF_BUFFER);
        return 0;
    }

    // EOI
    if (context->m_DstPosition + 2 > context->m_DstSize) {
        SetError(context, ERROR_SHORT_OF_BUFFER);
        return 0;
    }
    context->m_Dst[context->m_DstPosition] = 0xFF;
    context->m_Dst[context->m_DstPosition + 1] = 0xD9;
    context->m_DstPosition += 2;
    return context->m_DstPosition;
}

// 0x004710F0
u32 StartJpegEncoderCore(JpegMpEncoderWorkObj* work, u8* dst, u32 dstSize, const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, bool isAddThumbnail, JpegMpEncoderTemporarySettingObj* settings)
{
    JpegMpEncoderContext* context = &work->m_Context;
    u8* const end = dst + dstSize;
    u32 stride = settings->m_Stride;
    if (reinterpret_cast<uptr>(src) & 3) {
        SetError(context, ERROR_MISALIGNED);
        return 0;
    }
    if (settings->m_NintendoData != NULL) {
        // the camera images
        if (dstSize >= 0x32000 || !((width == 640 && height == 480) || (width == 160 && height == 120)) || !isAddThumbnail) {
            SetError(context, ERROR_INVALID_PARAMETER);
            return 0;
        }
    }
    work->m_IsAddThumbnail = isAddThumbnail;

    bool isValidSize = false;
    if (width != 0 && width < 0x10000 && height != 0 && height < 0x10000) {
        switch (sampling) {
        case PIXEL_SAMPLING_YUV444:
            isValidSize = ((width | height) & 7) == 0;
            break;
        case PIXEL_SAMPLING_YUV420:
            isValidSize = ((width | height) & 15) == 0;
            break;
        case PIXEL_SAMPLING_YUV422:
            isValidSize = ((width & 15) | (height & 7)) == 0;
            break;
        }
    }
    if (!isValidSize) {
        SetError(context, ERROR_INVALID_PARAMETER);
        return 0;
    }

    if (format == PIXEL_FORMAT_YUYV8) {
        stride &= ~1;
    } else if (format == PIXEL_FORMAT_CTR_RGB565_BLOCK8 || format == PIXEL_FORMAT_CTR_RGB8_BLOCK8 || format == PIXEL_FORMAT_CTR_RGBA8_BLOCK8) {
        stride &= ~7;
    }
    if (stride < width) {
        stride = width;
    }
    // the thumbnail is encoded at the start of the buffer first
    u8* const buffer = reinterpret_cast<u8*>((reinterpret_cast<uptr>(dst) + 3) & ~3);
    detail::PreEncodeJpegApp1(work, settings, isAddThumbnail);
    if (isAddThumbnail) {
        u32 thumbnailWidth = settings->m_ThumbnailWidth;
        u32 thumbnailHeight = settings->m_ThumbnailHeight;
        u8 thumbnailSampling = settings->m_ThumbnailSampling;
        bool isValidThumbnail = false;
        if (thumbnailWidth != 0 && thumbnailWidth < 0x10000 && thumbnailHeight != 0 && thumbnailHeight < 0x10000) {
            switch (thumbnailSampling) {
            case PIXEL_SAMPLING_YUV444:
                isValidThumbnail = ((thumbnailWidth | thumbnailHeight) & 7) == 0;
                break;
            case PIXEL_SAMPLING_YUV420:
                isValidThumbnail = ((thumbnailWidth | thumbnailHeight) & 15) == 0;
                break;
            case PIXEL_SAMPLING_YUV422:
                isValidThumbnail = ((thumbnailWidth & 15) | (thumbnailHeight & 7)) == 0;
                break;
            }
        }
        if (!isValidThumbnail) {
            thumbnailWidth = 160;
            thumbnailHeight = 120;
            thumbnailSampling = PIXEL_SAMPLING_YUV422;
        }
        if (buffer + 0x89E > end) {
            SetError(context, ERROR_SHORT_OF_BUFFER);
            return 0;
        }
        // all formats but YUYV are resampled into CtrRgb565
        PixelFormat thumbnailFormat = format;
        switch (format) {
        case PIXEL_FORMAT_CTR_RGB565_BLOCK8:
        case PIXEL_FORMAT_RGB8:
        case PIXEL_FORMAT_CTR_RGB8_BLOCK8:
        case PIXEL_FORMAT_RGBA8:
        case PIXEL_FORMAT_CTR_RGBA8_BLOCK8:
        case PIXEL_FORMAT_BGR8:
        case PIXEL_FORMAT_ABGR8:
            thumbnailFormat = PIXEL_FORMAT_CTR_RGB565;
            break;
        default:
            break;
        }
        u8* const thumbnail = buffer + 0x89C;
        thumbnail[0] = 0xFF;
        thumbnail[1] = 0xD8;
        JpegMpEncoderContext* thumbnailContext = reinterpret_cast<JpegMpEncoderContext*>(buffer + 0x200);
        thumbnailContext->m_Quality = 0;
        bool isOk = InitializeJpegMpEncoderContext(thumbnailContext, thumbnail + 2, end - thumbnail - 2, buffer, thumbnailWidth, thumbnailHeight,
                                                   quality < THUMBNAIL_QUALITY_MAX ? quality : THUMBNAIL_QUALITY_MAX,
                                                   static_cast<PixelSampling>(thumbnailSampling), thumbnailFormat, 16, settings);
        u32 thumbnailSize = 0;
        if (isOk) {
            thumbnailContext->m_ConvertFunc = thumbnailContext->m_McuFunc;
            thumbnailContext->m_McuFunc = s_ResampleFuncs[format];
            isOk = thumbnailContext->m_McuFunc != NULL;
        }
        if (isOk) {
            thumbnailContext->m_ResampleBuffer = reinterpret_cast<u16*>(buffer);
            thumbnailContext->m_ResampleSource = static_cast<const u8*>(src);
            thumbnailContext->m_ResampleSourceWidth = width;
            thumbnailContext->m_ResampleSourceHeight = height;
            thumbnailContext->m_ResampleSourceStride = stride;
            thumbnailSize = Yos_JPEGEncodeFast(thumbnailContext);
            isOk = thumbnailSize != 0;
        }
        if (!isOk) {
            s8 error = thumbnailContext->m_Error;
            if (error == 0) {
                error = ERROR_UNSUPPORTED;
            }
            SetError(context, error);
            return 0;
        }
        thumbnailSize += 2;
        if (thumbnailSize & 1) {
            if (thumbnail + thumbnailSize + 1 > end) {
                SetError(context, ERROR_SHORT_OF_BUFFER);
                return 0;
            }
            thumbnail[thumbnailSize] = 0;
            ++thumbnailSize;
        }
        work->m_App1Size = detail::CalcJpegMpEncoderApp1Size(work);
        work->m_ThumbnailSize = thumbnailSize;
        memmove(dst + 4 + work->m_App1Size, thumbnail, thumbnailSize);
    }

    if (!InitializeJpegMpEncoderContext(context, dst, dstSize, src, width, height, quality, sampling, format, stride, settings)) {
        return 0;
    }
    if (!detail::EncodeJpegApp1(work)) {
        return 0;
    }
    if (work->m_IsMp) {
        return context->m_DstPosition;
    }
    return Yos_JPEGEncodeFast(context);
}

// 0x00471530 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleBgr8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY)
{
    struct Reader
    {
        u16 operator()(const JpegMpEncoderContext* context, u32 y, u32 x) const
        {
            const u8* p = context->m_ResampleSource + (y * context->m_ResampleSourceStride + x) * 3;
            return ToRgb565(p[2], p[1], p[0]);
        }
    };
    ResampleToRgb565(context, mcuX, mcuY, Reader());
}

// 0x00471700 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleRgb8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY)
{
    struct Reader
    {
        u16 operator()(const JpegMpEncoderContext* context, u32 y, u32 x) const
        {
            const u8* p = context->m_ResampleSource + (y * context->m_ResampleSourceStride + x) * 3;
            return ToRgb565(p[0], p[1], p[2]);
        }
    };
    ResampleToRgb565(context, mcuX, mcuY, Reader());
}

// 0x004718C8 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleAbgr8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY)
{
    struct Reader
    {
        u16 operator()(const JpegMpEncoderContext* context, u32 y, u32 x) const
        {
            const u8* p = context->m_ResampleSource + (y * context->m_ResampleSourceStride + x) * 4;
            return ToRgb565(p[3], p[2], p[1]);
        }
    };
    ResampleToRgb565(context, mcuX, mcuY, Reader());
}

// 0x00471AB0 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleRgba8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY)
{
    struct Reader
    {
        u16 operator()(const JpegMpEncoderContext* context, u32 y, u32 x) const
        {
            const u8* p = context->m_ResampleSource + (y * context->m_ResampleSourceStride + x) * 4;
            return ToRgb565(p[0], p[1], p[2]);
        }
    };
    ResampleToRgb565(context, mcuX, mcuY, Reader());
}

// 0x00471C94 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleYuyv8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY)
{
    // pairs of pixels share U and V; the right pixel of a pair is read first and kept
    u32 right = 0;
    const s32 mcuWidth = context->m_SamplingH * 8;
    u32 row = context->m_SamplingV * 8;
    u32 y = (mcuY + 1) * row;
    const u32 sourceWidth = context->m_ResampleSourceWidth;
    const u32 width = context->m_Width;
    const u32 stepX = (sourceWidth << 4) / width;
    const u32 startX = ((mcuX + 1) * mcuWidth - width) * stepX + (sourceWidth << 4);
    while (row != 0) {
        --y;
        --row;
        const u32 line = (((context->m_ResampleSourceHeight * y) << 4) / context->m_Height >> 4) * context->m_ResampleSourceStride;
        u32 x = mcuWidth;
        u32 sourceX = startX;
        while (x != 0) {
            sourceX -= stepX;
            const u32 sx = sourceX >> 4;
            const u32 word = reinterpret_cast<const u32*>(context->m_ResampleSource)[((sx & ~1) + line) >> 1];
            const u32 u = word & 0xFF00;
            const u32 v = word & 0xFF000000;
            const u32 luma = ((sx & 1) ? (word >> 16) : word) & 0xFF;
            --x;
            if (x & 1) {
                right = luma | u | v;
            } else {
                const u32 uv = ((right >> 1) + (u >> 1) + (v >> 1)) & ~0xFF00FF;
                reinterpret_cast<u32*>(context->m_ResampleBuffer)[(x + row * 16) >> 1] = luma | uv | ((right & 0xFF) << 16);
            }
        }
    }
    FinishResample(context, mcuX, mcuY);
}

// 0x00471E08 | nintendogs:bytes [tier B]
bool InitializeJpegMpEncoderContext(JpegMpEncoderContext* context, u8* dst, u32 dstSize, const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, u32 stride, const JpegMpEncoderTemporarySettingObj* settings)
{
    // the quantization tables stay when the quality does not change
    const u8 lastQuality = context->m_Quality;
    memset(context, 0, (lastQuality != 0 && lastQuality == quality) ? offsetof(JpegMpEncoderContext, m_LumaQuantization) : sizeof(JpegMpEncoderContext));
    context->m_Width = width;
    context->m_Height = height;
    context->m_Stride = stride;
    context->m_Source = src;
    context->m_Dst = dst;
    context->m_DstSize = dstSize;
    if (quality == 0 || quality > 100) {
        SetError(context, ERROR_INVALID_PARAMETER);
        return false;
    }
    context->m_Quality = quality;
    switch (sampling) {
    case PIXEL_SAMPLING_YUV444:
        context->m_SamplingV = 1;
        context->m_SamplingH = 1;
        break;
    case PIXEL_SAMPLING_YUV420:
        context->m_SamplingV = 2;
        context->m_SamplingH = 2;
        break;
    case PIXEL_SAMPLING_YUV422:
        context->m_SamplingH = 2;
        context->m_SamplingV = 1;
        break;
    default:
        SetError(context, ERROR_UNSUPPORTED);
        return false;
    }
    const u8 samplingH = context->m_SamplingH;
    const u8 samplingV = context->m_SamplingV;
    context->m_BlocksPerMcu = samplingH * samplingV;
    context->m_McuCountX = static_cast<u16>(width) / (samplingH * 8);
    context->m_McuCountY = static_cast<u16>(height) / (samplingV * 8);
    context->m_Sampling = sampling;
    if (format >= PIXEL_FORMAT_COUNT) {
        SetError(context, ERROR_INVALID_PARAMETER);
        return false;
    }
    context->m_PixelFormat = format;
    context->m_McuFunc = s_ConvertFuncs[format][sampling - 1];
    if (context->m_McuFunc == NULL) {
        SetError(context, ERROR_UNSUPPORTED);
        return false;
    }
    context->m_LineStep2Minus16 = stride * 2 - 16;
    context->m_LineStep4Minus32 = stride * 4 - 32;
    context->m_LineStep4Minus64 = stride * 4 - 64;
    context->m_LineStep3 = stride * 3;
    context->m_LineStep2Minus32 = stride * 2 - 32;
    context->m_LineStep3Minus24 = (stride - 8) * 3;
    context->m_LineStep3Minus48 = (stride - 16) * 3;
    context->m_Y.m_DcTable = detail::s_LumaDcCodes;
    context->m_Cb.m_DcTable = detail::s_ChromaDcCodes;
    context->m_Cb.m_AcTable = detail::s_ChromaAcCodes;
    context->m_Y.m_AcTable = detail::s_LumaAcCodes;
    context->m_Cr.m_DcTable = detail::s_ChromaDcCodes;
    context->m_Cr.m_AcTable = detail::s_ChromaAcCodes;

    if (lastQuality != quality) {
        // scaling of the standard tables as in the IJG library
        const u32 scale = (quality < 50) ? 5000 / quality : 200 - quality * 2;
        for (s32 i = 0; i < 64; ++i) {
            u32 value = (s_LumaQuantization[i] * scale + 50) / 100;
            if (value == 0) {
                value = 1;
            } else if (value > 255) {
                value = 255;
            }
            context->m_LumaQuantization[i] = value;
            context->m_LumaReciprocal[i] = s_AanScale[i] / static_cast<u8>(value);
            value = (s_ChromaQuantization[i] * scale + 50) / 100;
            if (value == 0) {
                value = 1;
            } else if (value > 255) {
                value = 255;
            }
            context->m_ChromaQuantization[i] = value;
            context->m_ChromaReciprocal[i] = s_AanScale[i] / static_cast<u8>(value);
        }
    }
    context->m_IsNoHuffmanTable = (settings->m_Flags & 0x40000000) >> 30;
    return true;
}

// 0x00472120 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleCtrRgb565(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY)
{
    struct Reader
    {
        u16 operator()(const JpegMpEncoderContext* context, u32 y, u32 x) const
        {
            return reinterpret_cast<const u16*>(context->m_ResampleSource)[y * context->m_ResampleSourceStride + x];
        }
    };
    ResampleToRgb565(context, mcuX, mcuY, Reader());
}

// 0x004722A8 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleCtrRgb8Block8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY)
{
    struct Reader
    {
        u16 operator()(const JpegMpEncoderContext* context, u32 y, u32 x) const
        {
            const u8* p = context->m_ResampleSource + GetBlock8Index(context, x, y) * 3;
            return ToRgb565(p[2], p[1], p[0]);
        }
    };
    ResampleToRgb565(context, mcuX, mcuY, Reader());
}

// 0x004724E0 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleCtrRgba8Block8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY)
{
    struct Reader
    {
        u16 operator()(const JpegMpEncoderContext* context, u32 y, u32 x) const
        {
            const u8* p = context->m_ResampleSource + GetBlock8Index(context, x, y) * 4;
            return ToRgb565(p[3], p[2], p[1]);
        }
    };
    ResampleToRgb565(context, mcuX, mcuY, Reader());
}

// 0x00472710 | nintendogs:bytes [tier B]
void JpegMpEncoderCResampleCtrRgb565Block8(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY)
{
    struct Reader
    {
        u16 operator()(const JpegMpEncoderContext* context, u32 y, u32 x) const
        {
            return reinterpret_cast<const u16*>(context->m_ResampleSource)[GetBlock8Index(context, x, y)];
        }
    };
    ResampleToRgb565(context, mcuX, mcuY, Reader());
}

} // namespace CTR
} // namespace jpeg
} // namespace nn
