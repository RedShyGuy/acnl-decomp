#pragma once

#include "imgdb/JpegMpBaseDecoder.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {

// reads an MP file (the photos of both cameras) and extracts its images as JPEG files (member names are ours; the
// outputs of a decode are not used in this binary)
// RTTI N5imgdb9MpDecoderE @ 0x008D2C48
// vtable 0x009090A8 (vptr 0x009090B0), offset_to_top 0, 2 entries
class MpDecoder : public JpegMpBaseDecoder, private nn::util::ADLFireWall::NonCopyable<MpDecoder>
{
public:
    MpDecoder(const void* data, u32 size);
    virtual ~MpDecoder();

    bool ExtractExif(bool isThumbnail);
    bool ExtractExif();
    // the image of an MP entry as a JPEG file
    bool ExtractJpeg(int index, void* buffer, u32 size, u32& extractedSize);
    // the left image
    bool ExtractJpegL(void* buffer, u32 size, u32& extractedSize);
    // the buffer size for both images
    u32 ComputeExtractJpegSize();

    DECOMP_EMIT_COPY u32 GetRealWidth() const { return m_Decoder.GetLastWidth(); } // 0x00756880
    DECOMP_EMIT_COPY u32 GetRealHeight() const { return m_Decoder.GetLastHeight(); } // 0x00756888
    DECOMP_EMIT_COPY u32 GetWidth() const // 0x00756890
    {
        if (m_ShrinkLevel == 0)
        {
            return m_Decoder.GetLastWidth();
        }
        return (m_Decoder.GetLastWidth() + (1 << m_ShrinkLevel) - 1) >> m_ShrinkLevel;
    }
    DECOMP_EMIT_COPY u32 GetHeight() const // 0x007568D0
    {
        if (m_ShrinkLevel == 0)
        {
            return m_Decoder.GetLastHeight();
        }
        return (m_Decoder.GetLastHeight() + (1 << m_ShrinkLevel) - 1) >> m_ShrinkLevel;
    }

private:
    // a file with an MP index of at least two images
    bool IsMultiPicture(nn::jpeg::CTR::MpIndex& index)
    {
        return m_Decoder.GetMpIndex(&index, m_pData, m_DataSize) && index.m_HasVersion && index.m_NumberOfImages >= 2;
    }
    void ReleaseOutput()
    {
        if (m_IsAllocateOutput)
        {
            if (m_pOutputL != NULL)
            {
                GetAllocator()->Free(m_pOutputL);
                m_pOutputL = NULL;
            }
            if (m_pOutputR != NULL)
            {
                GetAllocator()->Free(m_pOutputR);
                m_pOutputR = NULL;
            }
            m_OutputSize = 0;
        }
    }

public:
    // (ImageDbImageValidatorBasic reads the limits)
    nn::jpeg::CTR::JpegMpDecoder m_Decoder;  // +0x08
    void* m_pWork;                           // +0x18
    const u8* m_pData;                       // +0x1C
    u32 m_DataSize;                          // +0x20
    s32 m_MinWidth;                          // +0x24
    s32 m_MinHeight;                         // +0x28
    s32 m_MaxWidth;                          // +0x2C
    s32 m_MaxHeight;                         // +0x30
    s32 m_ThumbnailMinWidth;                 // +0x34
    s32 m_ThumbnailMinHeight;                // +0x38
    s32 m_ThumbnailMaxWidth;                 // +0x3C
    s32 m_ThumbnailMaxHeight;                // +0x40
    u32 m_Stride;                            // +0x44
    s32 m_MaxShrinkLevel;                    // +0x48
    s32 m_ThumbnailMaxShrinkLevel;           // +0x4C
    bool m_IsShrinkable;                     // +0x50
    bool m_IsThumbnailShrinkable;            // +0x51
    bool m_IsAllocateOutput;                 // +0x52
    void* m_pOutputL;                        // +0x54
    void* m_pOutputR;                        // +0x58
    u32 m_OutputSize;                        // +0x5C
    u32 m_DecodedSize;                       // +0x60
    nn::jpeg::CTR::PixelFormat m_Format;     // +0x64
    s32 m_ShrinkLevel;                       // +0x68
};
ASSERT_SIZE(MpDecoder, 0x6C);

} // namespace imgdb
