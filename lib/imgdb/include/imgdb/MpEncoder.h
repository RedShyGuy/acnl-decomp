#pragma once

#include "imgdb/JpegMpBaseEncoder.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {

// encodes the images of both cameras as an MP file of at most 400 KB (member names are ours)
// RTTI N5imgdb9MpEncoderE @ 0x008D2C68
// vtable 0x009090B8 (vptr 0x009090C0), offset_to_top 0, 2 entries
class MpEncoder : public JpegMpBaseEncoder, private nn::util::ADLFireWall::NonCopyable<MpEncoder>
{
public:
    MpEncoder(const void* dataL, const void* dataR, nn::jpeg::CTR::PixelFormat format, int width, int height);
    virtual ~MpEncoder();

    bool Encode();

    Result m_Result;                       // +0x79
    bool m_IsRetryable;                    // +0x7A
    const void* m_pDataL;                  // +0x7C
    const void* m_pDataR;                  // +0x80
    nn::jpeg::CTR::PixelFormat m_Format;   // +0x84
    s32 m_Width;                           // +0x88
    s32 m_Height;                          // +0x8C
    void* m_pOutput;                       // +0x90
    u32 m_OutputSize;                      // +0x94
    u32 m_EncodedSize;                     // +0x98
};
ASSERT_SIZE(MpEncoder, 0xA0);

} // namespace imgdb
