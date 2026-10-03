#pragma once

// Helpers of the fs IPC classes (ipc::FileSystem, File, Directory); only for their sources.
// All names are ours. Translate descriptors after 3dbrew "IPC".

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace fs {
namespace ipc {

// the kernel puts the process id of the caller into the next word
const bit32 IPC_PROCESS_ID = 0x20;

inline bit32 StaticBufferDescriptor(size_t size, s32 index)
{
    return (size << 14) | (index << 10) | 2;
}

// a buffer the service reads (input)
inline bit32 ReadBufferDescriptor(size_t size)
{
    return (size << 4) | 0xA;
}

// a buffer the service writes (output)
inline bit32 WriteBufferDescriptor(size_t size)
{
    return (size << 4) | 0xC;
}

// the reply's result, or the error of the request
inline nn::Result Send(nn::Handle session, bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(session);
    if (result.IsSuccess()) {
        result = nn::Result(command[1]);
    }
    return result;
}

} // namespace ipc
} // namespace fs
} // namespace nn
