#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive_Directory.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {

// 0x00348794 slot 0x08 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::Directory::TrySetPriority(s32 priority)
{
    return GetIpcDirectory().SetPriority(priority);
}

// 0x003487D0 slot 0x04 | nintendogs:bytes
void nn::fs::CTR::MPCore::detail::FileServerArchive::Directory::Close()
{
    if (GetHandle().IsValid()) {
        nn::fs::ipc::Directory(GetHandle()).Close();
    }
    this->~Directory();
    s_DirectoryHeap.Free(this);
}

// 0x00348838 slot 0x00 | nintendogs:bytes
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::Directory::TryRead(s32* readCount, nn::fs::DirectoryEntry* entries,
                                                                              s32 count)
{
    return GetIpcDirectory().Read(readCount, entries, count);
}

// 0x003488B4 slot 0x10
// 0x00348884 slot 0x14 (deleting dtor)
nn::fs::CTR::MPCore::detail::FileServerArchive::Directory::~Directory()
{
    // ~HandleObject closes the directory session
}

// 0x00726A08 slot 0x0C (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::Directory::TryGetPriority(s32* priority) const
{
    return GetIpcDirectory().GetPriority(priority);
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
