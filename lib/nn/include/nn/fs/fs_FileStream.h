#pragma once

#include "decomp.h"
#include "nn/fs/detail/fs_FileBase.h"
#include "nn/fs/fs_IStream.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs10FileStreamE @ 0x008CDBC0
// vtable 0x008FBC74 (vptr 0x008FBC7C), offset_to_top 0, 18 entries
// vtable 0x008FBCEC (vptr 0x008FBCF4), offset_to_top -4, 14 entries
// A file opened for reading and writing: every function forwards to its FileBase (at +8). The
// output functions are declared again here (in this order), so they get slots 0x30 to 0x44 of
// the primary vtable. The names of the functions without a symbol are ours.
class FileStream : public ::nn::fs::IStream, public ::nn::fs::detail::FileBase, public ::nn::util::ADLFireWall::NonCopyable<nn::fs::FileStream>
{
public:
    FileStream() {}

    virtual ~FileStream(); // 0x00346004 slot 0x00 (calls 0x0011F63C)
    virtual nn::Result TrySeek(s64 offset, PositionBase base); // 0x00345FC0 slot 0x08
    virtual void Seek(s64 offset, PositionBase base); // 0x00345F10 slot 0x0C
    virtual nn::Result TryGetPosition(s64* position) const; // 0x00726850 slot 0x10
    virtual s64 GetPosition() const; // 0x00726848 slot 0x14
    virtual nn::Result TrySetPosition(s64 position); // 0x00349764 slot 0x18
    virtual void SetPosition(s64 position); // 0x00345EC0 slot 0x1C
    virtual nn::Result TryGetSize(s64* size) const; // 0x0013F9B0 slot 0x20
    virtual s64 GetSize() const; // 0x00726864 slot 0x24
    virtual nn::Result TryRead(s32* readSize, void* buffer, size_t size); // 0x0013E6D8 slot 0x28
    virtual s32 Read(void* buffer, size_t size); // 0x00345EE4 slot 0x2C | fefates:bytes
    virtual s32 Write(const void* buffer, size_t size, bool flush); // 0x00345F60 slot 0x30
    virtual nn::Result TryWrite(s32* writtenSize, const void* buffer, size_t size, bool flush); // 0x00345FD8 slot 0x34
    virtual void SetSize(s64 size); // 0x00345F9C slot 0x38
    virtual nn::Result TrySetSize(s64 size); // 0x00349714 slot 0x3C
    virtual void Flush(); // 0x00345F3C slot 0x40
    virtual nn::Result TryFlush(); // 0x003498BC slot 0x44
};
ASSERT_SIZE(FileStream, 0x1C);
} // namespace fs
} // namespace nn
