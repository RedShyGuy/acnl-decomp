#include "nn/fs/fs_FileStream.h"
#include "nn/err/CTR/CTR_Api.h"

// The thunks of the secondary vtables (0x00778058 to 0x007780BC for IOutputStream at +4,
// 0x00779DA4 to 0x0077A480 for the virtual base IPositionable) are made by the compiler.

namespace nn {
namespace fs {

// 0x0011F63C (the base object destructor of ARMCC, called with a flag in r1)
// 0x00346004 slot 0x00
// 0x00345FF0 slot 0x04 (deleting dtor)
nn::fs::FileStream::~FileStream()
{
    // ~FileBase closes the file
}

// 0x0013E6D8 slot 0x28 (falls through into FileBase::TryRead)
nn::Result nn::fs::FileStream::TryRead(s32* readSize, void* buffer, size_t size)
{
    return FileBase::TryRead(readSize, buffer, size);
}

// 0x0013F9B0 slot 0x20 (falls through into FileBase::TryGetSize)
nn::Result nn::fs::FileStream::TryGetSize(s64* size) const
{
    return FileBase::TryGetSize(size);
}

// 0x00345EC0 slot 0x1C (name is ours)
void nn::fs::FileStream::SetPosition(s64 position)
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TrySetPosition(position));
}

// 0x00345EE4 slot 0x2C | fefates:bytes
s32 nn::fs::FileStream::Read(void* buffer, size_t size)
{
    s32 readSize;
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TryRead(&readSize, buffer, size));
    return readSize;
}

// 0x00345F10 slot 0x0C (name is ours)
void nn::fs::FileStream::Seek(s64 offset, PositionBase base)
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TrySeek(offset, base));
}

// 0x00345F3C slot 0x40 (name is ours)
void nn::fs::FileStream::Flush()
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TryFlush());
}

// 0x00345F60 slot 0x30 (name is ours)
s32 nn::fs::FileStream::Write(const void* buffer, size_t size, bool flush)
{
    s32 writtenSize;
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TryWrite(&writtenSize, buffer, size, flush));
    return writtenSize;
}

// 0x00345F9C slot 0x38 (name is ours)
void nn::fs::FileStream::SetSize(s64 size)
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TrySetSize(size));
}

// 0x00345FC0 slot 0x08
nn::Result nn::fs::FileStream::TrySeek(s64 offset, PositionBase base)
{
    return FileBase::TrySeek(offset, base);
}

// 0x00345FD8 slot 0x34
nn::Result nn::fs::FileStream::TryWrite(s32* writtenSize, const void* buffer, size_t size, bool flush)
{
    return FileBase::TryWrite(writtenSize, buffer, size, flush);
}

// 0x00349714 slot 0x3C (falls through into FileBase::TrySetSize)
nn::Result nn::fs::FileStream::TrySetSize(s64 size)
{
    return FileBase::TrySetSize(size);
}

// 0x00349764 slot 0x18 (falls through into FileBase::TrySetPosition)
nn::Result nn::fs::FileStream::TrySetPosition(s64 position)
{
    return FileBase::TrySetPosition(position);
}

// 0x003498BC slot 0x44 (falls through into FileBase::TryFlush)
nn::Result nn::fs::FileStream::TryFlush()
{
    return FileBase::TryFlush();
}

// 0x00726848 slot 0x14 (name is ours)
s64 nn::fs::FileStream::GetPosition() const
{
    return FileBase::GetPosition();
}

// 0x00726850 slot 0x10 (name is ours)
nn::Result nn::fs::FileStream::TryGetPosition(s64* position) const
{
    return FileBase::TryGetPosition(position);
}

// 0x00726864 slot 0x24 (name is ours)
s64 nn::fs::FileStream::GetSize() const
{
    s64 size;
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TryGetSize(&size));
    return size;
}

} // namespace fs
} // namespace nn
