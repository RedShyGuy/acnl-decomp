#pragma once

#include "imgdb/JpegMpBaseDecoder.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {

// decodes a JPEG file, shrunk to the size limits (member names are ours)
// RTTI N5imgdb11JpegDecoderE @ 0x008D2AA4
// vtable 0x00908EC0 (vptr 0x00908EC8), offset_to_top 0, 2 entries
class JpegDecoder : public JpegMpBaseDecoder, private nn::util::ADLFireWall::NonCopyable<JpegDecoder>
{
public:
    JpegDecoder(const void* data, u32 size);
    virtual ~JpegDecoder();

    bool ExtractExif(bool isThumbnail);
    bool ExtractExif();
    bool Decode(nn::jpeg::CTR::PixelFormat format);
    bool Decode(nn::jpeg::CTR::PixelFormat format, bool isThumbnail);

    const void* GetOutput() const { return m_pOutput; }
    DECOMP_EMIT_COPY u32 GetRealWidth() const { return m_Decoder.GetLastWidth(); } // 0x007372EC
    DECOMP_EMIT_COPY u32 GetRealHeight() const { return m_Decoder.GetLastHeight(); } // 0x0073731C
    // the size after shrinking
    DECOMP_EMIT_COPY u32 GetWidth() const // 0x00754CE8
    {
        if (m_ShrinkLevel == 0)
        {
            return m_Decoder.GetLastWidth();
        }
        return (m_Decoder.GetLastWidth() + (1 << m_ShrinkLevel) - 1) >> m_ShrinkLevel;
    }
    DECOMP_EMIT_COPY u32 GetHeight() const // 0x00754D28
    {
        if (m_ShrinkLevel == 0)
        {
            return m_Decoder.GetLastHeight();
        }
        return (m_Decoder.GetLastHeight() + (1 << m_ShrinkLevel) - 1) >> m_ShrinkLevel;
    }

private:
    void ReleaseOutput()
    {
        if (m_IsAllocateOutput)
        {
            if (m_pOutput != NULL)
            {
                GetAllocator()->Free(m_pOutput);
                m_pOutput = NULL;
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
    u32 m_Stride;                            // +0x44, pixels per output line (0: the width)
    s32 m_MaxShrinkLevel;                    // +0x48
    s32 m_ThumbnailMaxShrinkLevel;           // +0x4C
    bool m_IsShrinkable;                     // +0x50
    bool m_IsThumbnailShrinkable;            // +0x51
    bool m_IsAllocateOutput;                 // +0x52
    void* m_pOutput;                         // +0x54
    u32 m_OutputSize;                        // +0x58
    u32 m_DecodedSize;                       // +0x5C
    nn::jpeg::CTR::PixelFormat m_Format;     // +0x60
    s32 m_ShrinkLevel;                       // +0x64
};
ASSERT_SIZE(JpegDecoder, 0x68);

} // namespace imgdb
