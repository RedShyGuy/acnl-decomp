#include "nn/jpeg/CTR/jpeg_JpegMpEncoder.h"
#include <string.h>
#include "nn/fnd/fnd_DateTime.h"
#include "nn/jpeg/CTR/CTR_Api.h"
#include "nn/jpeg/CTR/detail/detail_Api.h"
#include "nn/nstd/nstd_String.h"

namespace nn {
namespace jpeg {
namespace CTR {
namespace {
const s8 ERROR_IMAGE_COUNT = -6;
const s8 ERROR_MP_TYPE = -7;
const s8 ERROR_MP_STATE = -8;
const s8 ERROR_SHORT_OF_BUFFER = -9;
const size_t IMAGE_COUNT_MAX = 4096;

inline void SetError(detail::JpegMpEncoderWorkObj* work, s8 error)
{
    if (work->m_Context.m_Error == 0) {
        work->m_Context.m_Error = error;
    }
}

// clears the work memory and sets up the MP tables behind it; returns its end
inline u8* InitializeWork(detail::JpegMpEncoderWorkObj* work)
{
    const u32 maxImages = work->m_MaxImageCount;
    memset(work, 0, JpegMpEncoder::GetWorkBufferSize(maxImages));
    u8* end = reinterpret_cast<u8*>(work) + sizeof(detail::JpegMpEncoderWorkObj);
    if (maxImages != 0) {
        work->m_MpEntries = reinterpret_cast<detail::JpegMpEncoderMpEntry*>(end);
        work->m_ImageUniqueIds = reinterpret_cast<char*>(end + maxImages * sizeof(detail::JpegMpEncoderMpEntry));
        work->m_MaxImageCount = maxImages;
        end = reinterpret_cast<u8*>(work->m_ImageUniqueIds) + maxImages * 33;
        detail::InitializeJpegMpEncoderApp2IndexTagWork(work);
        detail::InitializeJpegMpEncoderApp2AttributeTagWork(work);
    }
    detail::InitializeJpegMpEncoderApp1TagWork(work);
    return end;
}
} // namespace

// 0x0047057C | nintendogs:bytes [tier B]
bool nn::jpeg::CTR::JpegMpEncoder::Initialize(void* buffer, size_t size, size_t maxImages)
{
    m_IsInitialized = false;
    if (reinterpret_cast<uptr>(buffer) & 3) {
        return false;
    }
    const size_t required = GetWorkBufferSize(maxImages);
    if (required == 0 || required > size) {
        return false;
    }
    m_Work = static_cast<detail::JpegMpEncoderWorkObj*>(buffer);
    m_Work->m_MaxImageCount = maxImages;
    u8* end = InitializeWork(m_Work);
    memset(&m_Settings, 0, sizeof(m_Settings));
    ClearTemporarySetting();
    if (static_cast<size_t>(end - static_cast<u8*>(buffer)) != required) {
        return false;
    }
    m_IsInitialized = true;
    return true;
}

// 0x00470688 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::SetMakerNote(const u8* data, size_t size, u32 index)
{
    if (!m_IsInitialized || index >= 4) {
        return;
    }
    if (data == NULL || size == 0) {
        m_Settings.m_MakerNotes[index].m_Data = NULL;
        m_Settings.m_MakerNotes[index].m_Size = 0;
    } else {
        m_Settings.m_MakerNotes[index].m_Data = data;
        m_Settings.m_MakerNotes[index].m_Size = size;
    }
}

// 0x004706C0 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::GetDateTimeNow(char* buffer)
{
    const nn::fnd::DateTime now = nn::fnd::DateTime::GetNow();
    const nn::fnd::DateTimeParameters parameters = now.GetParameters();
    nn::nstd::TSNPrintf(buffer, 20, "%04d:%02d:%02d %02d:%02d:%02d", parameters.year, parameters.month, parameters.day, parameters.hour,
                        parameters.minute, parameters.second);
    buffer[19] = '\0';
}

// 0x0047075C | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::SetUserMakerNote(const u8* data, size_t size)
{
    if (!m_IsInitialized) {
        return;
    }
    if (data == NULL || size == 0) {
        m_Settings.m_MakerNotes[0].m_Data = NULL;
        m_Settings.m_MakerNotes[0].m_Size = 0;
    } else {
        m_Settings.m_MakerNotes[0].m_Data = data;
        m_Settings.m_MakerNotes[0].m_Size = size;
    }
}

// 0x00470788 | nintendogs:bytes [tier B]
u32 nn::jpeg::CTR::JpegMpEncoder::StartJpegEncoder(u8* dst, size_t dstSize, const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, bool isAddThumbnail)
{
    if (!m_IsInitialized) {
        return 0;
    }
    m_Work->m_IsMp = false;
    const u32 size = StartJpegEncoderCore(m_Work, dst, dstSize, src, width, height, quality, sampling, format, isAddThumbnail, &m_Settings);
    ClearTemporarySetting();
    return size;
}

// 0x004707F0 (name is ours)
size_t nn::jpeg::CTR::JpegMpEncoder::GetWorkBufferSize(size_t maxImages)
{
    if (maxImages == 0) {
        return sizeof(detail::JpegMpEncoderWorkObj);
    }
    if (maxImages >= IMAGE_COUNT_MAX) {
        return 0;
    }
    // an MP entry and an image unique ID per image
    return sizeof(detail::JpegMpEncoderWorkObj) + maxImages * 16 + maxImages * 33;
}

// 0x00470820
u32 nn::jpeg::CTR::JpegMpEncoder::StartMpEncoderNext(const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, bool isAddThumbnail, MpTypeCode typeCode, bool isLastImage)
{
    if (!m_IsInitialized) {
        return 0;
    }
    detail::JpegMpEncoderWorkObj* work = m_Work;
    if (work->m_Context.m_Error != 0) {
        ClearTemporarySetting();
        return 0;
    }
    u8* const dst = work->m_Context.m_Dst;
    if (!work->m_IsMp || work->m_ImageCount <= work->m_CurrentImage) {
        work->m_Context.m_Error = ERROR_MP_STATE;
        ClearTemporarySetting();
        return 0;
    }
    // the images start at even offsets
    const u32 padding = work->m_Context.m_DstPosition & 1;
    const u32 start = work->m_Context.m_DstPosition + padding;
    if (work->m_Context.m_DstSize <= start) {
        work->m_Context.m_Error = ERROR_SHORT_OF_BUFFER;
        ClearTemporarySetting();
        return 0;
    }
    if (padding) {
        dst[work->m_Context.m_DstPosition] = 0;
    }
    switch (typeCode) {
    case MP_TYPE_CODE_UNDEFINED:
    case MP_TYPE_CODE_MULTI_FRAME_PANORAMA:
    case MP_TYPE_CODE_MULTI_FRAME_DISPARITY:
    case MP_TYPE_CODE_MULTI_FRAME_MULTI_ANGLE:
        // all images of a multi-frame file have the type of the first
        if (m_Work->m_IsBaselinePrimary || m_Work->m_FirstTypeCode != typeCode) {
            SetError(work, ERROR_MP_TYPE);
            ClearTemporarySetting();
            return 0;
        }
        break;
    case MP_TYPE_CODE_LARGE_THUMBNAIL_VGA:
    case MP_TYPE_CODE_LARGE_THUMBNAIL_FULL_HD:
        break;
    default:
        SetError(work, ERROR_MP_TYPE);
        ClearTemporarySetting();
        return 0;
    }
    m_Work->m_TypeCode = typeCode;
    m_Work->m_IsLastImage = isLastImage;
    const u32 dstSize = work->m_Context.m_DstSize;
    if (!StartMpEncoderCore(m_Work, work->m_Context.m_Dst + work->m_Context.m_DstPosition + padding, dstSize - (work->m_Context.m_DstPosition + padding), src,
                            width, height, quality, sampling, format, isAddThumbnail, &m_Settings)) {
        ClearTemporarySetting();
        return 0;
    }
    // the encoder worked on the rest of the buffer: back to the whole file
    const u8* const imageDst = work->m_Context.m_Dst;
    work->m_Context.m_Dst = dst;
    work->m_Context.m_DstPosition += imageDst - dst;
    work->m_Context.m_DstSize = dstSize;
    ClearTemporarySetting();
    return work->m_Context.m_DstPosition;
}

// 0x004709D4
u32 nn::jpeg::CTR::JpegMpEncoder::StartMpEncoderFirst(u8* dst, size_t dstSize, const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, bool isAddThumbnail, u32 imageCount, MpTypeCode typeCode, bool hasImageUniqueIds, bool hasTotalFrames)
{
    if (!m_IsInitialized) {
        return 0;
    }
    if (imageCount == 0 || imageCount >= IMAGE_COUNT_MAX || m_Work->m_MaxImageCount < imageCount) {
        SetError(m_Work, ERROR_IMAGE_COUNT);
        ClearTemporarySetting();
        return 0;
    }
    InitializeWork(m_Work);
    switch (typeCode) {
    case MP_TYPE_CODE_UNDEFINED:
    case MP_TYPE_CODE_MULTI_FRAME_PANORAMA:
    case MP_TYPE_CODE_MULTI_FRAME_MULTI_ANGLE:
        break;
    case MP_TYPE_CODE_MULTI_FRAME_DISPARITY:
        if (imageCount < 2) {
            SetError(m_Work, ERROR_IMAGE_COUNT);
            ClearTemporarySetting();
            return 0;
        }
        break;
    case MP_TYPE_CODE_BASELINE_PRIMARY:
        // the primary image with up to two large thumbnails
        if (imageCount > 3) {
            SetError(m_Work, ERROR_IMAGE_COUNT);
            ClearTemporarySetting();
            return 0;
        }
        m_Work->m_IsBaselinePrimary = true;
        hasTotalFrames = false;
        break;
    default:
        SetError(m_Work, ERROR_MP_TYPE);
        ClearTemporarySetting();
        return 0;
    }
    m_Work->m_ImageCount = imageCount;
    m_Work->m_TypeCode = typeCode;
    m_Work->m_FirstTypeCode = typeCode;
    m_Work->m_HasImageUniqueIds = hasImageUniqueIds;
    m_Work->m_HasTotalFrames = hasTotalFrames;
    if (!StartMpEncoderCore(m_Work, dst, dstSize, src, width, height, quality, sampling, format, isAddThumbnail, &m_Settings)) {
        ClearTemporarySetting();
        return 0;
    }
    ClearTemporarySetting();
    return m_Work->m_Context.m_DstPosition;
}

// 0x0047AC08 | nintendogs:bytes [tier B]
void nn::jpeg::CTR::JpegMpEncoder::ClearTemporarySetting()
{
    // the date stays (only its flag is cleared)
    char dateTime[20];
    memcpy(dateTime, m_Settings.m_DateTime, sizeof(dateTime));
    memset(&m_Settings, 0, sizeof(m_Settings));
    m_Settings.m_ThumbnailWidth = 160;
    m_Settings.m_ThumbnailHeight = 120;
    m_Settings.m_ThumbnailSampling = PIXEL_SAMPLING_YUV422;
    memcpy(m_Settings.m_DateTime, dateTime, sizeof(dateTime));
    detail::InitializeJpegMpEncoderApp1TagWork(m_Work);
}

// 0x007374C8 | nintendogs:bytes [tier B]
s32 nn::jpeg::CTR::JpegMpEncoder::GetLastError() const
{
    if (!m_IsInitialized) {
        return -1;
    }
    return m_Work->m_Context.m_Error;
}

} // namespace CTR
} // namespace jpeg
} // namespace nn
