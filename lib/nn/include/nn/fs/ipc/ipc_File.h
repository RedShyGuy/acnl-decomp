#pragma once

// nn::fs::ipc::File - the commands of a file session (3dbrew "Filesystem services", FSFile:*).
// The member name is ours.

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/fs/fs_Types.h"

namespace nn {
namespace fs {
namespace ipc {
class File
{
public:
    explicit File(nn::Handle session) : mSession(session) {}

    nn::Result GetPriority(s32* priority); // 0x003493A8 (name after 3dbrew)
    nn::Result OpenSubFile(nn::Handle* file, s64 offset, s64 size); // 0x003493E0 (name after 3dbrew)
    nn::Result SetPriority(s32 priority); // 0x0034942C | nintendogs:bytes [tier A]
    nn::Result OpenLinkFile(nn::Handle* file); // 0x0034945C | nintendogs:bytes [tier A]
    nn::Result Read(s32* readSize, s64 offset, void* buffer, size_t size); // 0x00349494 | nintendogs:bytes [tier A]
    nn::Result Close(); // 0x003494EC | nintendogs:bytes [tier A]
    nn::Result Write(s32* writtenSize, s64 offset, const void* buffer, size_t size, nn::fs::WriteOption option); // 0x00349518 | nintendogs:bytes [tier A]
    nn::Result GetAvailable(s64* available, s64 offset, s64 size); // 0x00349578 (name after 3dbrew)
    nn::Result GetSize(s64* size); // 0x003495C4 | nintendogs:bytes [tier A]
    // symbols.json: nn::fs::ipc::FsFile::SetSize (fefates:callgraph, tier C); the command is
    // FSFile:SetSize of this session
    nn::Result SetSize(s64 size); // 0x003495FC (name after 3dbrew)

private:
    nn::Handle mSession;
};
} // namespace ipc
} // namespace fs
} // namespace nn
