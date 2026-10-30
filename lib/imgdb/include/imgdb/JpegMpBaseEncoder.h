#pragma once

#include "imgdb/SysMakerNote.h"
#include "imgdb/imgdb_Types.h"
#include "nn/fnd/fnd_DateTime.h"
#include "nn/jpeg/CTR/jpeg_JpegMpEncoder.h"

namespace imgdb {

// the settings and Exif data of JpegEncoder and MpEncoder (names are ours)
// RTTI N5imgdb17JpegMpBaseEncoderE @ 0x008D2B74
// vtable 0x00908FB0 (vptr 0x00908FB8): both destructor slots are __cxa_pure_virtual
class JpegMpBaseEncoder
{
public:
    JpegMpBaseEncoder()
        : m_Sampling(nn::jpeg::CTR::PIXEL_SAMPLING_YUV422), m_Quality(80), m_IsAddThumbnail(true),
          m_ThumbnailSampling(nn::jpeg::CTR::PIXEL_SAMPLING_YUV422), m_ThumbnailWidth(160), m_ThumbnailHeight(120),
          m_IsFitThumbnail(true), m_pUserMakerNote(NULL), m_UserMakerNoteSize(0), m_pSoftware(NULL),
          m_HasTitleUniqueId(false)
    {
        m_DateTime = nn::fnd::DateTime::GetNow();
        m_SysMakerNote.Initialize();
    }
    virtual ~JpegMpBaseEncoder() = 0;

    // a maker note of the system ("3DS1", 64 bytes); false for another one
    bool SetSysMakerNote(const void* data);
    bool SetSysMakerNote(const SysMakerNote& note);
    void SetSysMakerNoteShootingType(ShootingType type);
    void SetUserMakerNoteData(const void* data, u32 size);
    void SetSysMakerNoteBodyId(BodyIdType type, u32 id);
    // five lower case hex digits
    static void ConvTitleUniqueIdToString(char* buffer, int size, u32 titleUniqueId);
    void SetSysMakerNoteCreateSecond(const DateTimeSeconds& seconds);
    void SetSysMakerNoteTitleUniqueId(u32 titleUniqueId);

protected:
    // the size of the thumbnails that keep the aspect ratio
    static const int FIT_THUMBNAIL_WIDTH = 160;
    static const int FIT_THUMBNAIL_HEIGHT = 120;

    // a size rounded to whole blocks of the sampling, at most max
    static int AlignToBlock(int value, int block, int max)
    {
        int rest = value % block;
        if (rest != 0)
        {
            if (rest > block / 2)
            {
                value -= rest;
            }
            else
            {
                value += block - rest;
                while (value > max)
                {
                    value -= block;
                }
            }
        }
        return value;
    }

    void SetDateTime(nn::jpeg::CTR::JpegMpEncoder& encoder) const
    {
        DateTimeSeconds seconds;
        seconds.ConvertFromNNDateTime(m_DateTime);
        char string[20];
        seconds.ConvertToDateTimeString(string, sizeof(string));
        encoder.SetDateTime(string);
    }

    // the software tag and both maker notes
    void SetMakerNotes(nn::jpeg::CTR::JpegMpEncoder& encoder) const
    {
        if (m_pSoftware != NULL)
        {
            encoder.SetSoftware(m_pSoftware);
        }
        encoder.SetMakerNote(reinterpret_cast<const u8*>(&m_SysMakerNote), sizeof(m_SysMakerNote), 1);
        if (m_pUserMakerNote != NULL && m_UserMakerNoteSize != 0)
        {
            encoder.SetUserMakerNote(static_cast<const u8*>(m_pUserMakerNote), m_UserMakerNoteSize);
        }
    }

    void SetThumbnail(nn::jpeg::CTR::JpegMpEncoder& encoder, int imageWidth, int imageHeight) const
    {
        int width;
        int height;
        if (m_IsFitThumbnail)
        {
            width = imageWidth;
            height = imageHeight;
            while (width > FIT_THUMBNAIL_WIDTH || height > FIT_THUMBNAIL_HEIGHT)
            {
                width >>= 1;
                height >>= 1;
            }
            int blockWidth;
            int blockHeight;
            if (m_ThumbnailSampling == nn::jpeg::CTR::PIXEL_SAMPLING_YUV444)
            {
                blockWidth = 8;
                blockHeight = 8;
            }
            else
            {
                blockWidth = m_ThumbnailSampling == nn::jpeg::CTR::PIXEL_SAMPLING_YUV420 ||
                                     m_ThumbnailSampling == nn::jpeg::CTR::PIXEL_SAMPLING_YUV422
                                 ? 16
                                 : 0;
                if (m_ThumbnailSampling == nn::jpeg::CTR::PIXEL_SAMPLING_YUV420)
                {
                    blockHeight = 16;
                }
                else
                {
                    blockHeight = m_ThumbnailSampling == nn::jpeg::CTR::PIXEL_SAMPLING_YUV422 ? 8 : 0;
                }
            }
            width = AlignToBlock(width, blockWidth, FIT_THUMBNAIL_WIDTH);
            height = AlignToBlock(height, blockHeight, FIT_THUMBNAIL_HEIGHT);
        }
        else
        {
            width = m_ThumbnailWidth;
            height = m_ThumbnailHeight;
        }
        encoder.SetThumbnailSize(width, height, m_ThumbnailSampling);
    }

public:
    // (the savers set the fields directly)
    nn::jpeg::CTR::PixelSampling m_Sampling;           // +0x04
    s32 m_Quality;                                     // +0x08
    bool m_IsAddThumbnail;                             // +0x0C
    nn::jpeg::CTR::PixelSampling m_ThumbnailSampling;  // +0x0D
    s32 m_ThumbnailWidth;                              // +0x10
    s32 m_ThumbnailHeight;                             // +0x14
    bool m_IsFitThumbnail;                             // +0x18, the size from the image instead
    nn::fnd::DateTime m_DateTime;                      // +0x20
    const void* m_pUserMakerNote;                      // +0x28
    u32 m_UserMakerNoteSize;                           // +0x2C
    const char* m_pSoftware;                           // +0x30
    u32 _34;                                           // +0x34
    SysMakerNote m_SysMakerNote;                       // +0x38
    bool m_HasTitleUniqueId;                           // +0x78
};

inline JpegMpBaseEncoder::~JpegMpBaseEncoder()
{
}

} // namespace imgdb
