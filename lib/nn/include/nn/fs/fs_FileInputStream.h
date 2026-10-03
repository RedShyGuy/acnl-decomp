#pragma once

#include "decomp.h"
#include "nn/fs/detail/fs_FileBase.h"
#include "nn/fs/fs_IInputStream.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace nn {
namespace fs {
// RTTI N2nn2fs15FileInputStreamE @ 0x008CDC20
// vtable 0x008FBD64 (vptr 0x008FBD6C), offset_to_top 0, 12 entries
// A file opened for reading: every function forwards to its FileBase (at +4). The names of the
// functions without a symbol are ours.
class FileInputStream : public ::nn::fs::IInputStream, public ::nn::fs::detail::FileBase, public ::nn::util::ADLFireWall::NonCopyable<nn::fs::FileInputStream>
{
public:
    // inline (e.g. in nn::ubl)
    FileInputStream() {}
    nn::Result TryInitialize(const wchar_t* path) { return FileBase::TryInitialize(path, OPEN_MODE_READ); }

    virtual ~FileInputStream(); // 0x0013E5F8 slot 0x00 (then 0x0013E600 | fefates:bytes [tier B])
    virtual nn::Result TrySeek(s64 offset, PositionBase base); // 0x00346380 slot 0x08
    virtual void Seek(s64 offset, PositionBase base); // 0x0034634C slot 0x0C
    virtual nn::Result TryGetPosition(s64* position) const; // 0x007268A0 slot 0x10
    virtual s64 GetPosition() const; // 0x00726898 slot 0x14
    virtual nn::Result TrySetPosition(s64 position); // 0x00346318 slot 0x18
    virtual void SetPosition(s64 position); // 0x003462F4 slot 0x1C
    virtual nn::Result TryGetSize(s64* size) const; // 0x00726890 slot 0x20
    virtual s64 GetSize() const; // 0x007268B4 slot 0x24
    virtual nn::Result TryRead(s32* readSize, void* buffer, size_t size); // 0x00346378 slot 0x28
    virtual s32 Read(void* buffer, size_t size); // 0x00346320 slot 0x2C | fefates:bytes
};
ASSERT_SIZE(FileInputStream, 0x18);
} // namespace fs
} // namespace nn
