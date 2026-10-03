#include "nn/fs/ipc/ipc_File.h"
#include "nn/fs/ipc/ipc_Common.h"

namespace nn {
namespace fs {
namespace ipc {
namespace {

// command headers, 3dbrew "FSFile:*"
const bit32 COMMAND_OPEN_SUB_FILE = 0x08010100;
const bit32 COMMAND_READ = 0x080200C2;
const bit32 COMMAND_WRITE = 0x08030102;
const bit32 COMMAND_GET_SIZE = 0x08040000;
const bit32 COMMAND_SET_SIZE = 0x08050080;
const bit32 COMMAND_CLOSE = 0x08080000;
const bit32 COMMAND_SET_PRIORITY = 0x080A0040;
const bit32 COMMAND_GET_PRIORITY = 0x080B0000;
const bit32 COMMAND_OPEN_LINK_FILE = 0x080C0000;
const bit32 COMMAND_GET_AVAILABLE = 0x0C010100;

} // namespace

// 0x003493A8 (name after 3dbrew)
nn::Result nn::fs::ipc::File::GetPriority(s32* priority)
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

// 0x003493E0 (name after 3dbrew)
nn::Result nn::fs::ipc::File::OpenSubFile(nn::Handle* file, s64 offset, s64 size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_OPEN_SUB_FILE;
    *reinterpret_cast<s64*>(&command[1]) = offset;
    *reinterpret_cast<s64*>(&command[3]) = size;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *file = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x0034942C | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::File::SetPriority(s32 priority)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_PRIORITY;
    command[1] = priority;
    return Send(mSession, command);
}

// 0x0034945C | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::File::OpenLinkFile(nn::Handle* file)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_OPEN_LINK_FILE;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *file = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x00349494 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::File::Read(s32* readSize, s64 offset, void* buffer, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_READ;
    *reinterpret_cast<s64*>(&command[1]) = offset;
    command[3] = size;
    command[4] = WriteBufferDescriptor(size);
    command[5] = reinterpret_cast<uptr>(buffer);
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *readSize = command[2];
    return nn::Result(command[1]);
}

// 0x003494EC | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::File::Close()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CLOSE;
    return Send(mSession, command);
}

// 0x00349518 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::File::Write(s32* writtenSize, s64 offset, const void* buffer, size_t size, nn::fs::WriteOption option)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_WRITE;
    *reinterpret_cast<s64*>(&command[1]) = offset;
    command[3] = size;
    *reinterpret_cast<nn::fs::WriteOption*>(&command[4]) = option;
    command[5] = ReadBufferDescriptor(size);
    command[6] = reinterpret_cast<uptr>(buffer);
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *writtenSize = command[2];
    return nn::Result(command[1]);
}

// 0x00349578 (name after 3dbrew)
nn::Result nn::fs::ipc::File::GetAvailable(s64* available, s64 offset, s64 size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_AVAILABLE;
    *reinterpret_cast<s64*>(&command[1]) = offset;
    *reinterpret_cast<s64*>(&command[3]) = size;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *available = *reinterpret_cast<s64*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x003495C4 | nintendogs:bytes [tier A]
nn::Result nn::fs::ipc::File::GetSize(s64* size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_SIZE;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *size = *reinterpret_cast<s64*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x003495FC (name after 3dbrew; symbols.json: nn::fs::ipc::FsFile::SetSize, tier C)
nn::Result nn::fs::ipc::File::SetSize(s64 size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_SIZE;
    *reinterpret_cast<s64*>(&command[1]) = size;
    return Send(mSession, command);
}

} // namespace ipc
} // namespace fs
} // namespace nn
