#pragma once

#include "decomp.h"
#include "nn/fs/detail/fs_FileBase.h"
#include "nn/fs/fs_IOutputStream.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs16FileOutputStreamE @ 0x008CDC48
// vtable 0x008FBDD4 (vptr 0x008FBDDC), offset_to_top 0, 16 entries
// A file opened for writing: every function forwards to its FileBase (at +4). The names of the
// functions are ours.
class FileOutputStream : public ::nn::fs::IOutputStream, public ::nn::fs::detail::FileBase, public ::nn::util::ADLFireWall::NonCopyable<nn::fs::FileOutputStream>
{
public:
    // inline (e.g. in nn::ubl)
    FileOutputStream() {}
    nn::Result TryInitialize(const wchar_t* path) { return FileBase::TryInitialize(path, OPEN_MODE_WRITE); }

    virtual ~FileOutputStream(); // 0x00346510 slot 0x00
    virtual nn::Result TrySeek(s64 offset, PositionBase base); // 0x00346498 slot 0x08
    virtual void Seek(s64 offset, PositionBase base); // 0x003463E8 slot 0x0C
    virtual nn::Result TryGetPosition(s64* position) const; // 0x007268F0 slot 0x10
    virtual s64 GetPosition() const; // 0x007268E8 slot 0x14
    virtual nn::Result TrySetPosition(s64 position); // 0x003463E0 slot 0x18
    virtual void SetPosition(s64 position); // 0x003463BC slot 0x1C
    virtual nn::Result TryGetSize(s64* size) const; // 0x007268E0 slot 0x20
    virtual s64 GetSize() const; // 0x00726904 slot 0x24
    virtual nn::Result TryWrite(s32* writtenSize, const void* buffer, size_t size, bool flush); // 0x003464B8 slot 0x28
    virtual s32 Write(const void* buffer, size_t size, bool flush); // 0x00346438 slot 0x2C
    virtual nn::Result TrySetSize(s64 size); // 0x003463B4 slot 0x30
    virtual void SetSize(s64 size); // 0x00346474 slot 0x34
    virtual void Flush(); // 0x00346414 slot 0x38
    virtual nn::Result TryFlush(); // 0x003464B0 slot 0x3C
};
ASSERT_SIZE(FileOutputStream, 0x18);
} // namespace fs
} // namespace nn
