#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive_File.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {

// 0x003471A8 slot 0x20
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::OpenSubFile(nn::Handle* file, s64 offset, s64 size)
{
    return mArchive->OpenSubFile(file, mBegin + offset, size);
}

// 0x003471E0 slot 0x30 | fefates:bytes [tier B]
void nn::fs::CTR::MPCore::detail::RomFsArchive::File::Close()
{
    this->~File();
    mArchive->mFileHeap.Free(this);
}

// 0x00347230 slot 0x00 | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryRead(s32* readSize, s64 offset, void* buffer, size_t size)
{
    s64 fileSize = mEnd - mBegin;
    if (offset + size > fileSize) {
        size = fileSize - offset;
    }
    return mArchive->GetFile(mPriority)->TryRead(readSize, mBegin + offset, buffer, size);
}

// 0x003472C8 slot 0x14
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryFlush()
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x003472D4 slot 0x04
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryWrite(s32* writtenSize, s64 offset, const void* buffer, size_t size, bool flush)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x003472E4 slot 0x34
// 0x003472E0 slot 0x38 (deleting dtor)
nn::fs::CTR::MPCore::detail::RomFsArchive::File::~File()
{
    // nothing to do
}

// 0x00347064 slot 0x10
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TrySetSize(s64 size)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x00347070 slot 0x24
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::OpenLinkHandle(nn::Handle* handle)
{
    return mArchive->OpenLinkHandle(handle);
}

// 0x00347080 slot 0x18 | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TrySetPriority(s32 priority)
{
    nn::Result result = mArchive->PrepareFile(priority);
    if (result.IsFailure()) {
        return result;
    }
    mPriority = priority;
    return nn::Result();
}

// 0x00348C0C slot 0x28
nn::Handle nn::fs::CTR::MPCore::detail::RomFsArchive::File::GetFileHandle()
{
    return nn::Handle(0);
}

// 0x00348C14 slot 0x2C
void nn::fs::CTR::MPCore::detail::RomFsArchive::File::DetachFileHandle()
{
    // nothing to do
}

// 0x00348C18 slot 0x08
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryGetAvailable(s64* available, s64 offset, s64 size)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x00726930 slot 0x0C | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryGetSize(s64* size) const
{
    *size = mEnd - mBegin;
    return nn::Result();
}

// 0x00726958 slot 0x1C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::File::TryGetPriority(s32* priority) const
{
    if (priority != 0) {
        *priority = mPriority;
    }
    return nn::Result();
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
