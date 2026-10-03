#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive_RomFsStorage.h"

#include <string.h>

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {

// 0x00346D0C | nintendogs:bytes [tier A]
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage::ReadBytes(s64 offset, void* buffer, size_t size)
{
    if (mBuffer != 0) {
        memcpy(buffer, mBuffer + offset, size);
    } else if (mArchive != 0) {
        s32 readSize;
        nn::Result result = mArchive->GetFile(mArchive->mPriority)->TryRead(&readSize, mOffset + static_cast<u32>(offset), buffer, size);
        if (result.IsFailure()) {
            return result;
        }
    }
    return nn::Result();
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
