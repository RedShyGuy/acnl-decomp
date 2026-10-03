#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive_File.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {

// 0x003483B0 slot 0x10 | fefates:bytes
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::File::TrySetSize(s64 size)
{
    return GetIpcFile().SetSize(size);
}

// 0x003483D8 slot 0x28
nn::Handle nn::fs::CTR::MPCore::detail::FileServerArchive::File::GetFileHandle()
{
    return GetHandle();
}

// 0x003483E0 slot 0x24
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::File::OpenLinkHandle(nn::Handle* handle)
{
    return GetIpcFile().OpenLinkFile(handle);
}

// 0x00348400 slot 0x18
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::File::TrySetPriority(s32 priority)
{
    return GetIpcFile().SetPriority(priority);
}

// 0x00348420 slot 0x20 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::File::OpenSubFile(nn::Handle* file, s64 offset, s64 size)
{
    return GetIpcFile().OpenSubFile(file, offset, size);
}

// 0x00348464 slot 0x2C (name is ours)
void nn::fs::CTR::MPCore::detail::FileServerArchive::File::DetachFileHandle()
{
    mHandle = nn::Handle();
}

// 0x00348470 slot 0x30 | nintendogs:bytes-fuzzy
void nn::fs::CTR::MPCore::detail::FileServerArchive::File::Close()
{
    if (GetHandle().IsValid()) {
        GetIpcFile().Close();
    }
    this->~File();
    s_FileHeap.Free(this);
}

// 0x003484E0 slot 0x00 | nintendogs:bytes
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::File::TryRead(s32* readSize, s64 offset, void* buffer, size_t size)
{
    return GetIpcFile().Read(readSize, offset, buffer, size);
}

// 0x00348520 slot 0x14 | nintendogs:bytes
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::File::TryFlush()
{
    // a write of 0 bytes that flushes
    nn::fs::WriteOption option;
    option.isFlush = true;
    option.unknown1 = false;
    option.unknown2 = true;
    option.unknown3 = 0;
    option.reserved = 0;
    s32 writtenSize;
    return GetIpcFile().Write(&writtenSize, 0, &writtenSize, 0, option);
}

// 0x003485A8 slot 0x04 | nintendogs:bytes
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::File::TryWrite(s32* writtenSize, s64 offset, const void* buffer,
                                                                          size_t size, bool flush)
{
    nn::fs::WriteOption option;
    option.isFlush = flush;
    option.unknown1 = false;
    option.unknown2 = flush;
    option.unknown3 = 0;
    option.reserved = 0;
    return GetIpcFile().Write(writtenSize, offset, buffer, size, option);
}

// 0x00348638 slot 0x08 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::File::TryGetAvailable(s64* available, s64 offset, s64 size)
{
    return GetIpcFile().GetAvailable(available, offset, size);
}

// 0x003486AC slot 0x34
// 0x0034867C slot 0x38 (deleting dtor)
nn::fs::CTR::MPCore::detail::FileServerArchive::File::~File()
{
    // ~HandleObject closes the file session
}

// 0x007269A0 slot 0x0C
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::File::TryGetSize(s64* size) const
{
    return GetIpcFile().GetSize(size);
}

// 0x007269C0 (name is ours)
DECOMP_NOINLINE nn::fs::ipc::File nn::fs::CTR::MPCore::detail::FileServerArchive::File::GetIpcFile() const
{
    if (!GetHandle().IsValid()) {
        nn::err::CTR::ThrowFatalErrAll(nn::Result(RESULT_NOT_OPENED), nn::err::CTR::GetCurrentAddress());
    }
    return nn::fs::ipc::File(GetHandle());
}

// 0x007269E8 slot 0x1C (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::FileServerArchive::File::TryGetPriority(s32* priority) const
{
    return GetIpcFile().GetPriority(priority);
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
