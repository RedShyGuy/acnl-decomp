#pragma once

#include <cstring>

#include "decomp.h"
#include "nn/jpeg/CTR/detail/jpeg_EncoderWork.h"
#include "nn/jpeg/CTR/jpeg_Types.h"

namespace nn {
namespace jpeg {
namespace CTR {
// Encodes JPEG files with Exif data, or MP files of several images (member names are ours).
// The settings (maker notes, ...) apply to the next encode only.
class JpegMpEncoder
{
public:
    // (inline: imgdb builds and finalizes encoders and sets the Exif data, names are ours)
    JpegMpEncoder() : m_IsInitialized(false) {}
    ~JpegMpEncoder() { Finalize(); }
    void Finalize() { m_IsInitialized = false; }
    // "YYYY:MM:DD hh:mm:ss"
    void SetDateTime(const char* dateTime)
    {
        if (m_IsInitialized)
        {
            std::memcpy(m_Settings.m_DateTime, dateTime, sizeof(m_Settings.m_DateTime));
            m_Settings.m_DateTime[sizeof(m_Settings.m_DateTime) - 1] = '\0';
            m_Settings.m_HasDateTime = true;
        }
    }
    void SetSoftware(const char* software)
    {
        if (m_IsInitialized)
        {
            m_Settings.m_Software = software;
        }
    }
    void SetNintendoData(const u8* data)
    {
        if (m_IsInitialized)
        {
            m_Settings.m_NintendoData = data;
        }
    }
    void SetThumbnailSize(u32 width, u32 height, PixelSampling sampling)
    {
        if (m_IsInitialized)
        {
            m_Settings.m_ThumbnailWidth = width;
            m_Settings.m_ThumbnailHeight = height;
            m_Settings.m_ThumbnailSampling = sampling;
        }
    }
    // the size of the work memory for MP files of up to maxImages images (0: plain JPEG)
    static size_t GetWorkBufferSize(size_t maxImages); // 0x004707F0 (name is ours)

    bool Initialize(void* buffer, size_t size, size_t maxImages); // 0x0047057C | nintendogs:bytes [tier B]
    void SetMakerNote(const u8* data, size_t size, u32 index); // 0x00470688 | nintendogs:bytes [tier B]
    void SetUserMakerNote(const u8* data, size_t size); // 0x0047075C | nintendogs:bytes [tier B]
    // "YYYY:MM:DD hh:mm:ss"
    static void GetDateTimeNow(char* buffer); // 0x004706C0 | nintendogs:bytes [tier B]
    u32 StartJpegEncoder(u8* dst, size_t dstSize, const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, bool isAddThumbnail); // 0x00470788 | nintendogs:bytes [tier B]
    u32 StartMpEncoderFirst(u8* dst, size_t dstSize, const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, bool isAddThumbnail, u32 imageCount, MpTypeCode typeCode, bool hasImageUniqueIds, bool hasTotalFrames); // 0x004709D4
    u32 StartMpEncoderNext(const void* src, u32 width, u32 height, u32 quality, PixelSampling sampling, PixelFormat format, bool isAddThumbnail, MpTypeCode typeCode, bool isLastImage); // 0x00470820
    s32 GetLastError() const; // 0x007374C8 | nintendogs:bytes [tier B]

private:
    void ClearTemporarySetting(); // 0x0047AC08 | nintendogs:bytes [tier B]

    detail::JpegMpEncoderWorkObj* m_Work;               // 0x00
    bool m_IsInitialized;                               // 0x04
    detail::JpegMpEncoderTemporarySettingObj m_Settings; // 0x08
};
ASSERT_SIZE(JpegMpEncoder, 0x100);
} // namespace CTR
} // namespace jpeg
} // namespace nn
