#pragma once

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/fs/detail/fs_DirectoryBaseImpl.h"

namespace nn {
namespace fs {
namespace detail {
// RTTI N2nn2fs6detail13DirectoryBaseE @ 0x008CDD30
// An open directory of the user file system (the IDirectory as void*). The member and Finalize
// are ours.
class DirectoryBase : public ::nn::fs::detail::DirectoryBaseImpl
{
public:
    DirectoryBase() : mHandle(0) {}

    // inline (in ~Directory)
    void Finalize()
    {
        nn::fs::CTR::MPCore::detail::UserFileSystem::CloseDirectory(mHandle);
        mHandle = 0;
    }

protected:
    void* mHandle;  // 0x00
};
} // namespace detail
} // namespace fs
} // namespace nn
