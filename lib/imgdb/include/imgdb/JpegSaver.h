#pragma once

#include "imgdb/JpegMpBaseSaver.h"
#include "nn/jpeg/CTR/jpeg_Types.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {

// encodes an image and saves it as a JPEG photo on the SD card (member names are ours)
// RTTI N5imgdb9JpegSaverE @ 0x008D2C28
// vtable 0x00909098 (vptr 0x009090A0), offset_to_top 0, 2 entries
class JpegSaver : public JpegMpBaseSaver, private nn::util::ADLFireWall::NonCopyable<JpegSaver>
{
public:
    JpegSaver();
    virtual ~JpegSaver();

    // the path of the saved file is kept (as "$_sdmc:/...")
    Result Save();
    void SetRaw(const void* data, int width, int height, nn::jpeg::CTR::PixelFormat format);
    Result ValidateParam() const;

    s32 m_Quality;                          // +0x14
    nn::jpeg::CTR::PixelSampling m_Sampling;  // +0x18
    bool m_IsRetryable;                     // +0x19
    const void* m_pRaw;                     // +0x1C
    s32 m_Width;                            // +0x20
    s32 m_Height;                           // +0x24
    nn::jpeg::CTR::PixelFormat m_Format;    // +0x28
    wchar_t* m_pPath;                       // +0x2C
};
ASSERT_SIZE(JpegSaver, 0x30);

} // namespace imgdb
