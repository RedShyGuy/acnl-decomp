#include "imgdb/ImageDbImageValidatorBasic.h"
#include "imgdb/ImageInfo.h"
#include "imgdb/JpegDecoder.h"
#include "imgdb/MpDecoder.h"
#include "imgdb/PhotoHeaderImageInfoReader.h"
#include "imgdb/imgdb_Api.h"

namespace imgdb {

namespace {

// the largest files without a thumbnail
const s64 MP_SIZE_MAX_WITHOUT_THUMBNAIL = 409600;
const s64 JPEG_SIZE_MAX_WITHOUT_THUMBNAIL = 204800;
// the size of an image without a thumbnail
const u32 WIDTH_MIN = 160;
const u32 WIDTH_MAX = 1024;
const s32 HEIGHT_MIN = 120;
const s32 HEIGHT_MAX = 768;

// the limits of the decoder for the thumbnail and for the image
template <typename T>
inline bool IsInDecoderLimits(const T& decoder, bool isThumbnail)
{
    if (isThumbnail)
    {
        return decoder.m_ShrinkLevel <= decoder.m_ThumbnailMaxShrinkLevel &&
               static_cast<s32>(decoder.GetWidth()) >= decoder.m_ThumbnailMinWidth &&
               decoder.m_ThumbnailMaxWidth >= static_cast<s32>(decoder.GetWidth()) &&
               static_cast<s32>(decoder.GetHeight()) >= decoder.m_ThumbnailMinHeight &&
               decoder.m_ThumbnailMaxHeight >= static_cast<s32>(decoder.GetHeight());
    }
    return decoder.m_ShrinkLevel <= decoder.m_MaxShrinkLevel && static_cast<s32>(decoder.GetWidth()) >= decoder.m_MinWidth &&
           decoder.m_MaxWidth >= static_cast<s32>(decoder.GetWidth()) &&
           static_cast<s32>(decoder.GetHeight()) >= decoder.m_MinHeight &&
           static_cast<s32>(decoder.GetHeight()) <= decoder.m_MaxHeight;
}

} // namespace

// 0x005AB924 | nintendogs:bytes [tier B]
bool imgdb::ImageDbImageValidatorBasic::ValidateMp(const void* data, u32 size, s64 fileSize)
{
    MpDecoder decoder(data, size);
    m_IsValid = false;
    m_HasThumbnail = decoder.ExtractExif(true);
    if (m_HasThumbnail)
    {
        if (!IsInDecoderLimits(decoder, true))
        {
            return false;
        }
    }
    else if (fileSize > MP_SIZE_MAX_WITHOUT_THUMBNAIL)
    {
        return false;
    }
    if (!decoder.ExtractExif(false) || !IsInDecoderLimits(decoder, false))
    {
        return false;
    }
    if (!m_HasThumbnail)
    {
        u32 width = decoder.GetRealWidth();
        s32 height = decoder.GetRealHeight();
        if (width - WIDTH_MIN > WIDTH_MAX - WIDTH_MIN || height < HEIGHT_MIN || height > HEIGHT_MAX)
        {
            return false;
        }
    }
    nn::fnd::DateTime dateTime;
    if (decoder.GetDateTime(dateTime))
    {
        m_DateTime.ConvertFromNNDateTime(dateTime);
    }
    else
    {
        m_DateTime = DateTimeSeconds::GetMinSeconds();
    }
    m_IsValid = true;
    m_FileSize = fileSize;
    return true;
}

// 0x005ABB04 | nintendogs:bytes [tier B]
bool imgdb::ImageDbImageValidatorBasic::ValidateJpeg(const void* data, u32 size, s64 fileSize)
{
    JpegDecoder decoder(data, size);
    m_IsValid = false;
    m_HasThumbnail = decoder.ExtractExif(true);
    if (m_HasThumbnail)
    {
        if (!IsInDecoderLimits(decoder, true))
        {
            return false;
        }
    }
    else if (fileSize > JPEG_SIZE_MAX_WITHOUT_THUMBNAIL)
    {
        return false;
    }
    if (!decoder.ExtractExif(false) || !IsInDecoderLimits(decoder, false))
    {
        return false;
    }
    if (!m_HasThumbnail)
    {
        u32 width = decoder.GetRealWidth();
        s32 height = decoder.GetRealHeight();
        if (width - WIDTH_MIN > WIDTH_MAX - WIDTH_MIN || height < HEIGHT_MIN || height > HEIGHT_MAX)
        {
            return false;
        }
    }
    nn::fnd::DateTime dateTime;
    if (decoder.GetDateTime(dateTime))
    {
        m_DateTime.ConvertFromNNDateTime(dateTime);
    }
    else
    {
        m_DateTime = DateTimeSeconds::GetMinSeconds();
    }
    m_IsValid = true;
    m_FileSize = fileSize;
    return true;
}

// (AVI files are not checked)
// 0x005ABCE4 | nintendogs:bytes [tier B]
bool imgdb::ImageDbImageValidatorBasic::ValidateImage(StorageType storage, const ImageInfo& info, bool isLegacy)
{
    m_IsValid = false;
    if (info.GetImageKind() == IMAGE_KIND_AVI)
    {
        return false;
    }
    PhotoHeaderImageInfoReader reader(*GetAllocator(), storage, info, isLegacy);
    if (reader.ReadImage() != RESULT_SUCCESS)
    {
        return false;
    }
    ImageKind kind = info.GetImageKind();
    const void* data = reader.GetData();
    u32 size = reader.GetDataSize();
    s64 fileSize = reader.GetFileSize();
    m_IsValid = false;
    if (kind == IMAGE_KIND_JPEG)
    {
        return ValidateJpeg(data, size, fileSize);
    }
    if (kind == IMAGE_KIND_MP)
    {
        return ValidateMp(data, size, fileSize);
    }
    return false;
}

// 0x005ABDEC | nintendogs:bytes [tier B]
imgdb::ImageDbImageValidatorBasic::ImageDbImageValidatorBasic() : m_IsValid(false), m_FileSize(0), m_HasThumbnail(false)
{
    m_DateTime.Initialize();
}

} // namespace imgdb
