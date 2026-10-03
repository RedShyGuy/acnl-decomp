#pragma once

#include "decomp.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace fs {

// Names an extra save data: media type and save id (3dbrew "ExtSaveDataInfo"). The IPC commands
// copy it as 3 words; the save id is at +4, so it is no u64 member (that would be at +8).
// Member names are ours.
class ExtSaveDataSpecifier
{
public:
    void Make(nn::fs::MediaType mediaType, u64 saveId); // 0x001290C0 | nintendogs:bytes [tier A]

private:
    MediaType mMediaType;   // 0x0
    u8 mUnknown1;           // 0x1
    u16 mUnknown2;          // 0x2
    u32 mSaveIdLow;         // 0x4
    u32 mSaveIdHigh;        // 0x8

    static void CheckLayout()
    {
        ASSERT_OFFSET(ExtSaveDataSpecifier, mSaveIdLow, 0x4);
    }
};
ASSERT_SIZE(ExtSaveDataSpecifier, 0xC);

} // namespace fs
} // namespace nn
