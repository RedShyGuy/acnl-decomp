#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// RTTI N2nn2fs3CTR6MPCore6detail10IDirectoryE @ 0x008CDC70
// An open directory of an IArchive. The names of slots 0x08 and 0x0C are ours.
class IDirectory
{
public:
    virtual nn::Result TryRead(s32* readCount, nn::fs::DirectoryEntry* entries, s32 count) = 0; // slot 0x00
    // closes and destroys the directory, gives its memory back
    virtual void Close() = 0; // slot 0x04
    virtual nn::Result TrySetPriority(s32 priority) = 0; // slot 0x08
    virtual nn::Result TryGetPriority(s32* priority) const = 0; // slot 0x0C
    virtual ~IDirectory() {} // slots 0x10, 0x14
};
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
