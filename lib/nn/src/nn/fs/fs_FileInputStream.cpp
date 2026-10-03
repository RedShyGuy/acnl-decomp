#include "nn/fs/fs_FileInputStream.h"
#include "nn/err/CTR/CTR_Api.h"

namespace nn {
namespace fs {

// 0x0013E5F8 slot 0x00 (ARMCC: "mov r1, #0", then the destructor at 0x0013E600)
// 0x00346398 slot 0x04 (deleting dtor)
nn::fs::FileInputStream::~FileInputStream()
{
    // ~FileBase closes the file
}

// 0x003462F4 slot 0x1C (name is ours)
void nn::fs::FileInputStream::SetPosition(s64 position)
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TrySetPosition(position));
}

// 0x00346318 slot 0x18
nn::Result nn::fs::FileInputStream::TrySetPosition(s64 position)
{
    return FileBase::TrySetPosition(position);
}

// 0x00346320 slot 0x2C | fefates:bytes
s32 nn::fs::FileInputStream::Read(void* buffer, size_t size)
{
    s32 readSize;
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TryRead(&readSize, buffer, size));
    return readSize;
}

// 0x0034634C slot 0x0C (name is ours)
void nn::fs::FileInputStream::Seek(s64 offset, PositionBase base)
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TrySeek(offset, base));
}

// 0x00346378 slot 0x28
nn::Result nn::fs::FileInputStream::TryRead(s32* readSize, void* buffer, size_t size)
{
    return FileBase::TryRead(readSize, buffer, size);
}

// 0x00346380 slot 0x08
nn::Result nn::fs::FileInputStream::TrySeek(s64 offset, PositionBase base)
{
    return FileBase::TrySeek(offset, base);
}

// 0x00726890 slot 0x20
nn::Result nn::fs::FileInputStream::TryGetSize(s64* size) const
{
    return FileBase::TryGetSize(size);
}

// 0x00726898 slot 0x14 (name is ours)
s64 nn::fs::FileInputStream::GetPosition() const
{
    return FileBase::GetPosition();
}

// 0x007268A0 slot 0x10 (name is ours)
nn::Result nn::fs::FileInputStream::TryGetPosition(s64* position) const
{
    return FileBase::TryGetPosition(position);
}

// 0x007268B4 slot 0x24 (name is ours)
s64 nn::fs::FileInputStream::GetSize() const
{
    s64 size;
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TryGetSize(&size));
    return size;
}

} // namespace fs
} // namespace nn
