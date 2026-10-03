#include "nn/fs/fs_FileOutputStream.h"
#include "nn/err/CTR/CTR_Api.h"

namespace nn {
namespace fs {

// 0x003463B4 slot 0x30
nn::Result nn::fs::FileOutputStream::TrySetSize(s64 size)
{
    return FileBase::TrySetSize(size);
}

// 0x003463BC slot 0x1C (name is ours)
void nn::fs::FileOutputStream::SetPosition(s64 position)
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TrySetPosition(position));
}

// 0x003463E0 slot 0x18
nn::Result nn::fs::FileOutputStream::TrySetPosition(s64 position)
{
    return FileBase::TrySetPosition(position);
}

// 0x003463E8 slot 0x0C (name is ours)
void nn::fs::FileOutputStream::Seek(s64 offset, PositionBase base)
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TrySeek(offset, base));
}

// 0x00346414 slot 0x38 (name is ours)
void nn::fs::FileOutputStream::Flush()
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TryFlush());
}

// 0x00346438 slot 0x2C (name is ours)
s32 nn::fs::FileOutputStream::Write(const void* buffer, size_t size, bool flush)
{
    s32 writtenSize;
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TryWrite(&writtenSize, buffer, size, flush));
    return writtenSize;
}

// 0x00346474 slot 0x34 (name is ours)
void nn::fs::FileOutputStream::SetSize(s64 size)
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TrySetSize(size));
}

// 0x00346498 slot 0x08
nn::Result nn::fs::FileOutputStream::TrySeek(s64 offset, PositionBase base)
{
    return FileBase::TrySeek(offset, base);
}

// 0x003464B0 slot 0x3C
nn::Result nn::fs::FileOutputStream::TryFlush()
{
    return FileBase::TryFlush();
}

// 0x003464B8 slot 0x28
nn::Result nn::fs::FileOutputStream::TryWrite(s32* writtenSize, const void* buffer, size_t size, bool flush)
{
    return FileBase::TryWrite(writtenSize, buffer, size, flush);
}

// 0x00346510 slot 0x00
// 0x003464D0 slot 0x04 (deleting dtor)
nn::fs::FileOutputStream::~FileOutputStream()
{
    // ~FileBase closes the file
}

// 0x007268E0 slot 0x20
nn::Result nn::fs::FileOutputStream::TryGetSize(s64* size) const
{
    return FileBase::TryGetSize(size);
}

// 0x007268E8 slot 0x14 (name is ours)
s64 nn::fs::FileOutputStream::GetPosition() const
{
    return FileBase::GetPosition();
}

// 0x007268F0 slot 0x10 (name is ours)
nn::Result nn::fs::FileOutputStream::TryGetPosition(s64* position) const
{
    return FileBase::TryGetPosition(position);
}

// 0x00726904 slot 0x24 (name is ours)
s64 nn::fs::FileOutputStream::GetSize() const
{
    s64 size;
    nn::err::CTR::ThrowFatalErrAllIfFailure(FileBase::TryGetSize(&size));
    return size;
}

} // namespace fs
} // namespace nn
