#pragma once

#include "imgdb/JpegMpBaseSaver.h"
#include "nn/jpeg/CTR/jpeg_Types.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {

// encodes the images of both cameras and saves them as an MP photo on the SD card (member names are ours)
// RTTI N5imgdb7MpSaverE @ 0x008D2C08
// vtable 0x00909088 (vptr 0x00909090), offset_to_top 0, 2 entries
class MpSaver : public JpegMpBaseSaver, private nn::util::ADLFireWall::NonCopyable<MpSaver>
{
public:
    MpSaver();
    virtual ~MpSaver();

    Result Save();
    void SetRaw(const void* dataL, const void* dataR, int width, int height, nn::jpeg::CTR::PixelFormat format);
    Result ValidateParam() const;

    s32 m_Quality;                          // +0x14
    nn::jpeg::CTR::PixelSampling m_Sampling;  // +0x18
    bool m_IsRetryable;                     // +0x19
    const void* m_pRawL;                    // +0x1C
    const void* m_pRawR;                    // +0x20
    s32 m_Width;                            // +0x24
    s32 m_Height;                           // +0x28
    nn::jpeg::CTR::PixelFormat m_Format;    // +0x2C
    f32 _30;                                // +0x30
    wchar_t* m_pPath;                       // +0x34
};
ASSERT_SIZE(MpSaver, 0x38);

} // namespace imgdb
