// ac_Ac.cpp (the name is ours; static initializer at 0x007A04B0: s_Session)
#include "nn/ac/CTR/detail/ac_Ac.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace ac {
namespace CTR {
namespace detail {
namespace {
// command headers (3dbrew "AC Services")
const bit32 COMMAND_CREATE_DEFAULT_CONFIG = 0x00010000;
const bit32 COMMAND_CONNECT_ASYNC = 0x00040006;
const bit32 COMMAND_GET_CONNECT_RESULT = 0x00050002;
const bit32 COMMAND_CLOSE_ASYNC = 0x00080004;
const bit32 COMMAND_GET_CLOSE_RESULT = 0x00090002;
const bit32 COMMAND_GET_LAST_ERROR_CODE = 0x000A0000;
const bit32 COMMAND_GET_LAST_DETAIL_ERROR_CODE = 0x000B0000;
const bit32 COMMAND_GET_CONNECTING_LOCATION = 0x00140002;
const bit32 COMMAND_ADD_DENY_AP_TYPE = 0x00240042;
const bit32 COMMAND_GET_INFRA_PRIORITY = 0x00270002;
const bit32 COMMAND_SET_REQUEST_EULA_VERSION = 0x002D0082;
const bit32 COMMAND_REGISTER_DISCONNECT_EVENT = 0x00300004;
const bit32 COMMAND_IS_CONNECTED = 0x003E0042;
const bit32 COMMAND_SET_CLIENT_VERSION = 0x00400042;

// the translation descriptors (3dbrew "IPC")
const bit32 DESCRIPTOR_PROCESS_ID = 0x20;
const bit32 DESCRIPTOR_COPY_HANDLE = 0;

const size_t LOCATION_SIZE = 11;

inline bit32 StaticBufferDescriptor(size_t size, s32 index)
{
    return (size << 14) | (index << 10) | 2;
}

inline void SetByte(bit32* word, u8 value)
{
    *reinterpret_cast<u8*>(word) = value;
}

inline nn::Result Send(bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// a request whose reply goes into a buffer of the caller
inline nn::Result SendWithReceiveBuffer(bit32* command, void* buffer, size_t size)
{
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(size, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(buffer);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}
} // namespace

// 0x0097E7DC
nn::Handle s_Session;

// 0x003459A0 | fefates:callgraph [tier C]
nn::Result nn::ac::CTR::detail::Ac::CloseAsync(nn::Handle event)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CLOSE_ASYNC;
    command[4] = event.GetPrintableBits();
    command[3] = DESCRIPTOR_COPY_HANDLE;
    command[1] = DESCRIPTOR_PROCESS_ID;
    return Send(command);
}

// 0x003459E8 | fefates:callgraph [tier C]
nn::Result nn::ac::CTR::detail::Ac::IsConnected(u32 unknown, bool* pIsConnected)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_IS_CONNECTED;
    command[1] = unknown;
    command[2] = DESCRIPTOR_PROCESS_ID;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pIsConnected = *reinterpret_cast<bool*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00345A34 | fefates:bytes [tier B]
nn::Result nn::ac::CTR::detail::Ac::ConnectAsync(const nnacConfig& config, nn::Handle event)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CONNECT_ASYNC;
    command[4] = event.GetPrintableBits();
    command[3] = DESCRIPTOR_COPY_HANDLE;
    command[1] = DESCRIPTOR_PROCESS_ID;
    command[6] = reinterpret_cast<uptr>(&config);
    command[5] = StaticBufferDescriptor(sizeof(nnacConfig), 1);
    return Send(command);
}

// 0x00345A8C | fefates:bytes [tier B]
nn::Result nn::ac::CTR::detail::Ac::AddDenyApType(const nnacConfig& config, nnacConfig* pConfig, nn::ac::CTR::ApType type)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_ADD_DENY_AP_TYPE;
    command[1] = type;
    command[3] = reinterpret_cast<uptr>(&config);
    command[2] = StaticBufferDescriptor(sizeof(nnacConfig), 0);
    return SendWithReceiveBuffer(command, pConfig, sizeof(nnacConfig));
}

// 0x00345AEC | fefates:callgraph [tier C]
nn::Result nn::ac::CTR::detail::Ac::GetCloseResult()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_CLOSE_RESULT;
    command[1] = DESCRIPTOR_PROCESS_ID;
    return Send(command);
}

// 0x00345B28 | fefates:callgraph [tier C]
nn::Result nn::ac::CTR::detail::Ac::GetConnectResult()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_CONNECT_RESULT;
    command[1] = DESCRIPTOR_PROCESS_ID;
    return Send(command);
}

// 0x00345B64 | fefates:bytes [tier B]
nn::Result nn::ac::CTR::detail::Ac::GetInfraPriority(const nnacConfig& config, nn::ac::CTR::InfraPriority* pPriority)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_INFRA_PRIORITY;
    command[2] = reinterpret_cast<uptr>(&config);
    command[1] = StaticBufferDescriptor(sizeof(nnacConfig), 1);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pPriority = *reinterpret_cast<nn::ac::CTR::InfraPriority*>(&command[2]);
    return nn::Result(command[1]);
}

// 0x00345BB4 | fefates:bytes [tier B]
nn::Result nn::ac::CTR::detail::Ac::GetLastErrorCode(unsigned int* pCode)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_LAST_ERROR_CODE;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pCode = command[2];
    return nn::Result(command[1]);
}

// 0x00345BF0 | fefates:callgraph [tier C]
nn::Result nn::ac::CTR::detail::Ac::SetClientVersion(u32 version)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_CLIENT_VERSION;
    command[1] = version;
    command[2] = DESCRIPTOR_PROCESS_ID;
    return Send(command);
}

// 0x00345C30 | fefates:bytes [tier B]
nn::Result nn::ac::CTR::detail::Ac::CreateDefaultConfig(nnacConfig* pConfig)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CREATE_DEFAULT_CONFIG;
    return SendWithReceiveBuffer(command, pConfig, sizeof(nnacConfig));
}

// 0x00345C80 (name is ours)
nn::Result nn::ac::CTR::detail::Ac::GetConnectingLocationEntry(unsigned char* pLocation)
{
    return GetConnectingLocation(pLocation);
}

// 0x00345C84 | fefates:bytes [tier B]
nn::Result nn::ac::CTR::detail::Ac::GetConnectingLocation(unsigned char* pLocation)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_CONNECTING_LOCATION;
    command[1] = DESCRIPTOR_PROCESS_ID;
    return SendWithReceiveBuffer(command, pLocation, LOCATION_SIZE);
}

// 0x00345CE0 | fefates:bytes [tier B]
nn::Result nn::ac::CTR::detail::Ac::SetRequestEulaVersion(const nnacConfig& config, nnacConfig* pConfig, unsigned char major, unsigned char minor)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_REQUEST_EULA_VERSION;
    SetByte(&command[1], major);
    SetByte(&command[2], minor);
    command[4] = reinterpret_cast<uptr>(&config);
    command[3] = StaticBufferDescriptor(sizeof(nnacConfig), 0);
    return SendWithReceiveBuffer(command, pConfig, sizeof(nnacConfig));
}

// 0x00345D50 (name after 3dbrew)
nn::Result nn::ac::CTR::detail::Ac::GetLastDetailErrorCode(unsigned int* pCode)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_LAST_DETAIL_ERROR_CODE;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pCode = command[2];
    return nn::Result(command[1]);
}

// 0x00345D8C | fefates:callgraph [tier C]
nn::Result nn::ac::CTR::detail::Ac::RegisterDisconnectEvent(nn::Handle event)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_REGISTER_DISCONNECT_EVENT;
    command[4] = event.GetPrintableBits();
    command[3] = DESCRIPTOR_COPY_HANDLE;
    command[1] = DESCRIPTOR_PROCESS_ID;
    return Send(command);
}

} // namespace detail
} // namespace CTR
} // namespace ac
} // namespace nn
