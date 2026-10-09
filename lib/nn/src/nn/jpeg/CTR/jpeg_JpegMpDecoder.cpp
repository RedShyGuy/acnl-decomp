#include "nn/jpeg/CTR/jpeg_JpegMpDecoder.h"
#include <string.h>
#include "nn/jpeg/CTR/detail/detail_Api.h"

namespace nn {
namespace jpeg {
namespace CTR {
namespace {
const s8 ERROR_INVALID_PARAMETER = -2;
const s8 ERROR_NO_MP = -33;
} // namespace

// 0x0046FF70 | nintendogs:bytes [tier B]
bool nn::jpeg::CTR::JpegMpDecoder::GetMpEntry(MpEntry* entry, const MpIndex* index, u32 number)
{
    const u32 offset = number * 16;
    if (index->m_EntrySize < offset + 16) {
        return false;
    }
    if (index->m_SrcSize < offset + index->m_EntryOffset + 16) {
        return false;
    }
    const u8* p = index->m_Src + offset + index->m_EntryOffset;
    const bool isLittleEndian = index->m_IsLittleEndian;
    entry->m_Attribute = detail::ReadU32(p, isLittleEndian);
    entry->m_Size = detail::ReadU32(p + 4, isLittleEndian);
    entry->m_Offset = detail::ReadU32(p + 8, isLittleEndian);
    if (entry->m_Offset != 0) {
        entry->m_Offset += index->m_TiffHeaderOffset;
    }
    entry->m_Dependent1 = detail::ReadU16(p + 12, isLittleEndian);
    entry->m_Dependent2 = detail::ReadU16(p + 14, isLittleEndian);
    entry->m_Base = index->m_Src;
    return true;
}

// 0x00470080 | nintendogs:bytes [tier B]
bool nn::jpeg::CTR::JpegMpDecoder::GetMpIndex(MpIndex* index, const u8* src, size_t size)
{
    if (!m_IsInitialized) {
        return false;
    }
    JpegMpDecoderContext* context = m_Work;
    if (!detail::InitializeJpegMpDecoderContext(context, NULL, 0, src, size, 0, 0, PIXEL_FORMAT_YUYV8, false, true, &m_Settings)) {
        return false;
    }
    detail::Mel_JPEGDecodeFast(context);
    if (context->m_Error != 0) {
        return false;
    }
    if (!context->m_HasMpIndex) {
        context->m_Error = ERROR_NO_MP;
        return false;
    }
    *index = context->m_MpIndex;
    return true;
}

// 0x00470148 | nintendogs:bytes [tier B]
bool nn::jpeg::CTR::JpegMpDecoder::Initialize(void* buffer, size_t size)
{
    m_IsInitialized = false;
    m_Settings.m_Stride = 0;
    m_Settings.m_Flags = 0;
    if (reinterpret_cast<uptr>(buffer) & 3) {
        return false;
    }
    if (size < sizeof(JpegMpDecoderContext)) {
        return false;
    }
    memset(buffer, 0, sizeof(JpegMpDecoderContext));
    m_Work = static_cast<JpegMpDecoderContext*>(buffer);
    m_IsInitialized = true;
    return true;
}

// 0x0047019C | nintendogs:bytes [tier B]
bool nn::jpeg::CTR::JpegMpDecoder::ExtractExif(const u8* src, size_t size, bool isThumbnail)
{
    if (!m_IsInitialized) {
        return false;
    }
    JpegMpDecoderContext* context = m_Work;
    bool isOk = detail::InitializeJpegMpDecoderContext(context, NULL, 0, src, size, 0, 0, PIXEL_FORMAT_YUYV8, isThumbnail, true, &m_Settings);
    if (isOk) {
        detail::Mel_JPEGDecodeFast(context);
        if (context->m_Error != 0) {
            isOk = false;
        }
    }
    return isOk;
}

// 0x00470218
size_t nn::jpeg::CTR::JpegMpDecoder::GetDstBufferSize(u32 width, u32 height, PixelFormat format)
{
    u32 bytesPerPixel = 0;
    switch (format) {
    case PIXEL_FORMAT_YUYV8:
        width = (width + 1) & ~1;
        bytesPerPixel = 2;
        break;
    case PIXEL_FORMAT_CTR_RGB565:
        bytesPerPixel = 2;
        break;
    case PIXEL_FORMAT_CTR_RGB565_BLOCK8:
        bytesPerPixel = 2;
        width = (width + 7) & ~7;
        height = (height + 7) & ~7;
        break;
    case PIXEL_FORMAT_RGB8:
    case PIXEL_FORMAT_BGR8:
        bytesPerPixel = 3;
        break;
    case PIXEL_FORMAT_CTR_RGB8_BLOCK8:
        bytesPerPixel = 3;
        width = (width + 7) & ~7;
        height = (height + 7) & ~7;
        break;
    case PIXEL_FORMAT_RGBA8:
    case PIXEL_FORMAT_ABGR8:
        bytesPerPixel = 4;
        break;
    case PIXEL_FORMAT_CTR_RGBA8_BLOCK8:
        bytesPerPixel = 4;
        width = (width + 7) & ~7;
        height = (height + 7) & ~7;
        break;
    default:
        break;
    }
    const u64 size = static_cast<u64>(bytesPerPixel * width) * height;
    if (size >> 32) {
        return 0;
    }
    return static_cast<size_t>(size);
}

// 0x004702D8 | nintendogs:bytes [tier B]
size_t nn::jpeg::CTR::JpegMpDecoder::StartJpegDecoder(void* dst, size_t dstSize, const u8* src, size_t srcSize, u32 maxWidth, u32 maxHeight, PixelFormat format, bool isThumbnail)
{
    if (!m_IsInitialized) {
        return 0;
    }
    JpegMpDecoderContext* context = m_Work;
    size_t size = 0;
    if (detail::InitializeJpegMpDecoderContext(context, dst, dstSize, src, srcSize, maxWidth, maxHeight, format, isThumbnail, false, &m_Settings)) {
        detail::Mel_JPEGDecodeFast(context);
        if (context->m_Error == 0) {
            size = GetDstBufferSize(context->m_Stride, context->m_AlignedHeight, format);
        }
    }
    m_Settings.m_Stride = 0;
    m_Settings.m_Flags = 0;
    return size;
}

// 0x00470374
size_t nn::jpeg::CTR::JpegMpDecoder::GetWorkBufferSize()
{
    return sizeof(JpegMpDecoderContext);
}

// 0x00470380 | nintendogs:bytes [tier B]
size_t nn::jpeg::CTR::JpegMpDecoder::StartJpegDecoderShrink(void* dst, size_t dstSize, const u8* src, size_t srcSize, u32 maxWidth, u32 maxHeight, PixelFormat format, bool isThumbnail, u32 shrink)
{
    if (!m_IsInitialized) {
        return 0;
    }
    JpegMpDecoderContext* context = m_Work;
    size_t size = 0;
    // a scale down by 2, 4, 8 or 16 of a size that divides
    const u32 mask = (1 << shrink) - 1;
    if (shrink - 1 >= 4 || (mask & maxWidth) || (mask & maxHeight)) {
        if (context->m_Error == 0) {
            context->m_Error = ERROR_INVALID_PARAMETER;
        }
    } else if (detail::InitializeJpegMpDecoderContext(context, dst, dstSize, src, srcSize, maxWidth >> shrink, maxHeight >> shrink, format, isThumbnail, false,
                                                      &m_Settings)) {
        context->m_Shrink = shrink;
        context->m_MaxWidth = maxWidth;
        context->m_MaxHeight = maxHeight;
        if (context->m_SettingFlags & 2) {
            context->m_ExpectedWidth = maxWidth;
            context->m_ExpectedHeight = maxHeight;
        }
        detail::Mel_JPEGDecodeFast(context);
        if (context->m_Error == 0) {
            size = GetDstBufferSize(context->m_Stride, context->m_AlignedHeight, format);
        }
    }
    m_Settings.m_Stride = 0;
    m_Settings.m_Flags = 0;
    return size;
}

// 0x00470490 | nintendogs:bytes [tier B]
bool nn::jpeg::CTR::JpegMpDecoder::GetMpRegionsToBuildJpegData(MpRegionsToBuildJpegData* regions, const u8* src, size_t size)
{
    if (!m_IsInitialized) {
        return false;
    }
    JpegMpDecoderContext* context = m_Work;
    if (!detail::InitializeJpegMpDecoderContext(context, NULL, 0, src, size, 0, 0, PIXEL_FORMAT_YUYV8, false, true, &m_Settings)) {
        return false;
    }
    detail::Mel_JPEGDecodeFast(context);
    if (context->m_Error != 0) {
        return false;
    }
    if (context->m_App2 == NULL) {
        context->m_Error = ERROR_NO_MP;
        return false;
    }
    // the file without its MP APP2 segment
    const u32 segmentSize = ((context->m_App2[2] << 8) | context->m_App2[3]) + 2;
    const u32 offset = context->m_App2 - src;
    const u32 rest = size - (offset + segmentSize);
    if (offset == 0 || rest == 0 || size <= offset + rest) {
        return false;
    }
    regions->m_Data2 = NULL;
    regions->m_Size1 = offset;
    regions->m_Size2 = 0;
    regions->m_Data1 = src;
    regions->m_Data2 = context->m_App2 + segmentSize;
    regions->m_Size2 = rest;
    return true;
}

// 0x004731C0 | nintendogs:bytes [tier B]
size_t nn::jpeg::CTR::JpegMpDecoder::GetLastDateTime(char* buffer) const
{
    if (!m_IsInitialized || m_Work->m_Error != 0) {
        return 0;
    }
    return detail::GetApp1DateTime(buffer, m_Work);
}

// 0x007372F4 | nintendogs:bytes [tier B]
u32 nn::jpeg::CTR::JpegMpDecoder::GetLastWidth() const
{
    if (!m_IsInitialized || m_Work->m_Error != 0) {
        return 0;
    }
    return m_Work->m_Width;
}

// 0x00737324 | nintendogs:bytes [tier B]
u32 nn::jpeg::CTR::JpegMpDecoder::GetLastHeight() const
{
    if (!m_IsInitialized || m_Work->m_Error != 0) {
        return 0;
    }
    return m_Work->m_Height;
}

// 0x0073734C | nintendogs:bytes [tier B]
size_t nn::jpeg::CTR::JpegMpDecoder::GetLastMakerNoteSize(u32 index) const
{
    detail::App1PointerAndSize makerNote;
    if (!m_IsInitialized || m_Work->m_Error != 0 || !detail::GetApp1MakerNotePointer(&makerNote, m_Work, index)) {
        return 0;
    }
    return makerNote.m_Size;
}

// 0x0073739C (name is ours)
const u8* nn::jpeg::CTR::JpegMpDecoder::GetLastSoftware() const
{
    detail::App1PointerAndSize software;
    if (!m_IsInitialized || m_Work->m_Error != 0 || !detail::GetApp1SoftwarePointer(&software, m_Work)) {
        return NULL;
    }
    return software.m_Pointer;
}

// 0x007373E0 | nintendogs:bytes [tier B]
const u8* nn::jpeg::CTR::JpegMpDecoder::GetLastMakerNotePointer(u32 index) const
{
    detail::App1PointerAndSize makerNote;
    if (!m_IsInitialized || m_Work->m_Error != 0 || !detail::GetApp1MakerNotePointer(&makerNote, m_Work, index)) {
        return NULL;
    }
    return makerNote.m_Pointer;
}

// 0x00737430 | nintendogs:bytes [tier B]
size_t nn::jpeg::CTR::JpegMpDecoder::GetLastUserMakerNoteSize() const
{
    detail::App1PointerAndSize makerNote;
    if (!m_IsInitialized || m_Work->m_Error != 0 || !detail::GetApp1MakerNotePointer(&makerNote, m_Work, 0)) {
        return 0;
    }
    return makerNote.m_Size;
}

// 0x00737480 | nintendogs:bytes [tier B]
const u8* nn::jpeg::CTR::JpegMpDecoder::GetLastUserMakerNotePointer() const
{
    detail::App1PointerAndSize makerNote;
    if (!m_IsInitialized || m_Work->m_Error != 0 || !detail::GetApp1MakerNotePointer(&makerNote, m_Work, 0)) {
        return NULL;
    }
    return makerNote.m_Pointer;
}

} // namespace CTR
} // namespace jpeg
} // namespace nn
