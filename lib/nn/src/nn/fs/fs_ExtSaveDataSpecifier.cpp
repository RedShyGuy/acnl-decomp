#include "nn/fs/fs_ExtSaveDataSpecifier.h"

namespace nn {
namespace fs {

// 0x001290C0 | nintendogs:bytes [tier A]
void nn::fs::ExtSaveDataSpecifier::Make(nn::fs::MediaType mediaType, u64 saveId)
{
    mUnknown2 = 0;
    mMediaType = mediaType;
    mUnknown1 = 0;
    mSaveIdLow = static_cast<u32>(saveId);
    mSaveIdHigh = static_cast<u32>(saveId >> 32);
}

} // namespace fs
} // namespace nn
