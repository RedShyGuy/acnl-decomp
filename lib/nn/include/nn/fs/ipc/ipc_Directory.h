#pragma once

// nn::fs::ipc::Directory - the commands of a directory session (3dbrew "Filesystem services",
// FSDir:*). The member name is ours.

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace fs {
namespace ipc {
class Directory
{
public:
    explicit Directory(nn::Handle session) : mSession(session) {}

    nn::Result GetPriority(s32* priority); // 0x0034962C (name after 3dbrew)
    nn::Result SetPriority(s32 priority); // 0x00349664 (name after 3dbrew)
    nn::Result Read(s32* readCount, nn::fs::DirectoryEntry* entries, s32 count); // 0x00349694 | nintendogs:bytes [tier A]
    nn::Result Close(); // 0x003496E8 | nintendogs:bytes [tier A]

private:
    nn::Handle mSession;
};
} // namespace ipc
} // namespace fs
} // namespace nn
