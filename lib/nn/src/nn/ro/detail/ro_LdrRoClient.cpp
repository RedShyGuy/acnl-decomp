#include "nn/ro/detail/ro_LdrRoClient.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace ro {
namespace detail {
namespace {
// command headers (3dbrew "LDR:RO")
const bit32 COMMAND_INITIALIZE = 0x000100C2;
const bit32 COMMAND_LOAD_CRR = 0x00020082;
const bit32 COMMAND_UNLOAD_CRR = 0x00030042;
const bit32 COMMAND_UNLOAD_CRO = 0x000500C2;
const bit32 COMMAND_SHUTDOWN = 0x00080042;
const bit32 COMMAND_LOAD_CRO_NEW = 0x000902C2;
// the translation header of a copied handle (the process)
const bit32 COPY_HANDLE = 0;
} // namespace

// 0x0097F05C (name is ours)
nn::Handle s_Session;

// 0x0013098C (name after 3dbrew)
nn::Result LdrRoClient::Initialize(nn::Handle process, uptr crs, size_t size, uptr mappedAddress)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_INITIALIZE;
    command[1] = crs;
    command[2] = size;
    command[3] = mappedAddress;
    command[4] = COPY_HANDLE;
    command[5] = process.GetPrintableBits();
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x00124238 (name after 3dbrew)
nn::Result LdrRoClient::LoadCRR(nn::Handle process, uptr crr, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_LOAD_CRR;
    command[1] = crr;
    command[2] = size;
    command[3] = COPY_HANDLE;
    command[4] = process.GetPrintableBits();
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x0013B300 (name after 3dbrew)
nn::Result LdrRoClient::UnloadCRR(nn::Handle process, uptr crr)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_UNLOAD_CRR;
    command[1] = crr;
    command[2] = COPY_HANDLE;
    command[3] = process.GetPrintableBits();
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x0034CED4 (name after 3dbrew)
nn::Result LdrRoClient::LoadCRO(size_t* fixedSize, nn::Handle process, uptr cro, uptr mappedAddress, size_t croSize, uptr data, u32 zero,
                                size_t dataSize, uptr bss, size_t bssSize, bool isAutoLink, u32 fixLevel, uptr crr)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_LOAD_CRO_NEW;
    command[1] = cro;
    command[2] = mappedAddress;
    command[3] = croSize;
    command[4] = data;
    command[5] = zero;
    command[6] = dataSize;
    command[7] = bss;
    command[8] = bssSize;
    // (a byte, the rest of the word stays)
    *reinterpret_cast<bool*>(&command[9]) = isAutoLink;
    command[10] = fixLevel;
    command[11] = crr;
    command[12] = COPY_HANDLE;
    command[13] = process.GetPrintableBits();
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *fixedSize = command[2];
    return nn::Result(command[1]);
}

// 0x0013E8B4 (name after 3dbrew)
nn::Result LdrRoClient::UnloadCRO(nn::Handle process, uptr mappedAddress, u32 zero, uptr cro)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_UNLOAD_CRO;
    command[1] = mappedAddress;
    command[2] = zero;
    command[3] = cro;
    command[4] = COPY_HANDLE;
    command[5] = process.GetPrintableBits();
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x0013B344 (name after 3dbrew)
nn::Result LdrRoClient::Shutdown(nn::Handle process, uptr crsMappedAddress)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SHUTDOWN;
    command[1] = crsMappedAddress;
    command[2] = COPY_HANDLE;
    command[3] = process.GetPrintableBits();
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

} // namespace detail
} // namespace ro
} // namespace nn
