#include "nn/fs/ipc/ipc_Directory.h"
#include "nn/fs/ipc/ipc_Common.h"

namespace nn {
namespace fs {
namespace ipc {
namespace {

// command headers, 3dbrew "FSDir:*"
const bit32 COMMAND_READ = 0x08010042;
const bit32 COMMAND_CLOSE = 0x08020000;
const bit32 COMMAND_SET_PRIORITY = 0x08030040;
const bit32 COMMAND_GET_PRIORITY = 0x08040000;

} // namespace

// 0x0034962C (name after 3dbrew)
nn::Result nn::fs::ipc::Directory::GetPriority(s32* priority)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_PRIORITY;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *priority = command[2];
    return nn::Result(command[1]);
}

// 0x00349664 (name after 3dbrew)
nn::Result nn::fs::ipc::Directory::SetPriority(s32 priority)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_PRIORITY;
    command[1] = priority;
    return Send(mSession, command);
}

// 0x00349694 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::Directory::Read(s32* readCount, nn::fs::DirectoryEntry* entries, s32 count)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_READ;
    command[1] = count;
    command[2] = WriteBufferDescriptor(count * sizeof(nn::fs::DirectoryEntry));
    command[3] = reinterpret_cast<uptr>(entries);
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *readCount = command[2];
    return nn::Result(command[1]);
}

// 0x003496E8 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::Directory::Close()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CLOSE;
    return Send(mSession, command);
}

} // namespace ipc
} // namespace fs
} // namespace nn
