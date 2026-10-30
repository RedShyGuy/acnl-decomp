#pragma once

#include "imgdb/JpegMpBaseEncoder.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {

// encodes an image as a JPEG file of at most 200 KB (member names are ours)
// RTTI N5imgdb11JpegEncoderE @ 0x008D2AC4
// vtable 0x00908ED0 (vptr 0x00908ED8), offset_to_top 0, 2 entries
class JpegEncoder : public JpegMpBaseEncoder, private nn::util::ADLFireWall::NonCopyable<JpegEncoder>
{
public:
    JpegEncoder(const void* data, nn::jpeg::CTR::PixelFormat format, int width, int height);
    virtual ~JpegEncoder();

    // isTwl: a JPEG for the DSi camera (smaller than 200 KB, a thumbnail and empty Nintendo data instead of the
    // maker notes)
    bool Encode(bool isTwl);

    Result m_Result;                       // +0x79
    bool m_IsRetryable;                    // +0x7A, a lower quality when the file gets too large
    const void* m_pData;                   // +0x7C
    nn::jpeg::CTR::PixelFormat m_Format;   // +0x80
    s32 m_Width;                           // +0x84
    s32 m_Height;                          // +0x88
    void* m_pOutput;                       // +0x8C
    u32 m_OutputSize;                      // +0x90
    u32 m_EncodedSize;                     // +0x94
};
ASSERT_SIZE(JpegEncoder, 0x98);

} // namespace imgdb
