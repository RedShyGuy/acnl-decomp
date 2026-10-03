#include "nn/socket/detail/socket_User.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace socket {
namespace detail {

namespace {
// command headers (id << 16 | normal parameters << 6 | translate parameters), 3dbrew "Socket
// Services"
const bit32 COMMAND_INITIALIZE_SOCKETS = 0x00010044;
const bit32 COMMAND_SOCKET = 0x000200C2;
const bit32 COMMAND_BIND = 0x00050084;
const bit32 COMMAND_CONNECT = 0x00060084;
const bit32 COMMAND_RECV_FROM = 0x00070104;
const bit32 COMMAND_RECV_FROM_OTHER = 0x00080102;
const bit32 COMMAND_SEND_TO = 0x00090106;
const bit32 COMMAND_SEND_TO_OTHER = 0x000A0106;
const bit32 COMMAND_CLOSE = 0x000B0042;
const bit32 COMMAND_SHUTDOWN = 0x000C0082;
const bit32 COMMAND_GET_HOST_BY_NAME = 0x000D0082;
const bit32 COMMAND_GET_ADDR_INFO = 0x000F0106;
const bit32 COMMAND_SET_SOCK_OPT = 0x00120104;
const bit32 COMMAND_FCNTL = 0x001300C2;
const bit32 COMMAND_POLL = 0x00140084;
const bit32 COMMAND_GET_HOST_ID = 0x00160000;
const bit32 COMMAND_GET_SOCK_NAME = 0x00170082;
const bit32 COMMAND_SHUTDOWN_SOCKETS = 0x00190000;
const bit32 COMMAND_GET_NETWORK_OPT = 0x001A00C0;
const bit32 COMMAND_SEND_TO_MULTI = 0x00200146;

// translate descriptors (3dbrew "IPC")
const bit32 IPC_PROCESS_ID = 0x20;
const bit32 IPC_COPY_HANDLE = 0;

inline bit32 StaticBufferDescriptor(size_t size, s32 index)
{
    return (size << 14) | (index << 10) | 2;
}

inline bit32 ReadBufferDescriptor(size_t size)
{
    return (size << 4) | 0xA;
}

inline bit32 WriteBufferDescriptor(size_t size)
{
    return (size << 4) | 0xC;
}
} // namespace

// 0x004883DC | fefates:bytes [tier A]
nn::Result nn::socket::detail::User::SetSockOpt(s32* result, s32 socket, s32 level, s32 name, const u8* value, s32 valueSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SET_SOCK_OPT;
    command[1] = socket;
    command[2] = level;
    command[3] = name;
    command[4] = valueSize;
    command[5] = IPC_PROCESS_ID;
    command[7] = StaticBufferDescriptor(valueSize, 9);
    command[8] = reinterpret_cast<uptr>(value);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x00488444 | fefates:bytes [tier A]
nn::Result nn::socket::detail::User::GetAddrInfo(s32* result, const char* node, size_t nodeSize, const char* service, size_t serviceSize, const u8* hints, s32 hintsSize, s32* count, u8* addrInfo, size_t addrInfoSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_ADDR_INFO;
    command[1] = nodeSize;
    command[2] = serviceSize;
    command[3] = hintsSize;
    command[4] = addrInfoSize;
    command[5] = StaticBufferDescriptor(nodeSize, 5);
    command[6] = reinterpret_cast<uptr>(node);
    command[7] = StaticBufferDescriptor(serviceSize, 6);
    command[8] = reinterpret_cast<uptr>(service);
    command[9] = StaticBufferDescriptor(hintsSize, 7);
    command[10] = reinterpret_cast<uptr>(hints);
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(addrInfoSize, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(addrInfo);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    *count = command[3];
    return nn::Result(command[1]);
}

// 0x0048850C | fefates:bytes [tier A]
nn::Result nn::socket::detail::User::GetSockName(s32* result, s32 socket, u8* address, size_t addressSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_SOCK_NAME;
    command[1] = socket;
    command[2] = addressSize;
    command[3] = IPC_PROCESS_ID;
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(addressSize, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(address);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x00488578 | fefates:bytes [tier A]
// data and address in static buffers (for small datagrams)
nn::Result nn::socket::detail::User::SendToSmall(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* address, size_t addressSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SEND_TO_OTHER;
    command[1] = socket;
    command[2] = size;
    command[3] = flags;
    command[4] = addressSize;
    command[5] = IPC_PROCESS_ID;
    command[7] = StaticBufferDescriptor(size, 2);
    command[8] = reinterpret_cast<uptr>(buffer);
    command[9] = StaticBufferDescriptor(addressSize, 1);
    command[10] = reinterpret_cast<uptr>(address);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x004885FC | fefates:callgraph [tier A]
// soc:U InitializeSockets: the shared memory the service works in
nn::Result nn::socket::detail::User::AttachProcess(nn::Handle sharedMemory, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_INITIALIZE_SOCKETS;
    command[1] = size;
    command[2] = IPC_PROCESS_ID;
    command[4] = IPC_COPY_HANDLE;
    command[5] = sharedMemory.GetPrintableBits();
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result(command[1]);
}

// 0x00488640 | tier C (confirmed by the code)
// soc:U ShutdownSockets
nn::Result nn::socket::detail::User::DetachProcess()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SHUTDOWN_SOCKETS;
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result(command[1]);
}

// 0x00488668 | fefates:bytes [tier A]
nn::Result nn::socket::detail::User::GetHostByName(s32* result, const char* name, size_t nameSize, u8* hostEntry, size_t hostEntrySize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_HOST_BY_NAME;
    command[1] = nameSize;
    command[2] = hostEntrySize;
    command[3] = StaticBufferDescriptor(nameSize, 3);
    command[4] = reinterpret_cast<uptr>(name);
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(hostEntrySize, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(hostEntry);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x004886E4 | fefates:bytes [tier B]
nn::Result nn::socket::detail::User::GetNetworkOpt(s32* result, s32 level, s32 name, u8* value, s32* valueSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_NETWORK_OPT;
    command[1] = level;
    command[2] = name;
    command[3] = *valueSize;
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(*valueSize, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(value);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    *valueSize = command[3];
    return nn::Result(command[1]);
}

// 0x00488760 | fefates:bytes [tier B]
// data and address into static buffers; *size: in the buffer size, out the received size
nn::Result nn::socket::detail::User::RecvFromSmall(s32* result, s32 socket, u8* buffer, s32* size, s32 flags, u8* address, size_t addressSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RECV_FROM_OTHER;
    command[1] = socket;
    command[2] = *size;
    command[3] = flags;
    command[4] = addressSize;
    command[5] = IPC_PROCESS_ID;
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    bit32 saved2 = staticBuffers[2];
    bit32 saved3 = staticBuffers[3];
    staticBuffers[0] = StaticBufferDescriptor(*size, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(buffer);
    staticBuffers[2] = StaticBufferDescriptor(addressSize, 0);
    staticBuffers[3] = reinterpret_cast<uptr>(address);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    staticBuffers[2] = saved2;
    staticBuffers[3] = saved3;
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    *size = command[3];
    return nn::Result(command[1]);
}

// 0x00488804 | fefates:bytes [tier A]
// one datagram to several addresses
nn::Result nn::socket::detail::User::SendToSmallMulti(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* addresses, size_t addressSize, size_t addressesSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SEND_TO_MULTI;
    command[1] = socket;
    command[2] = size;
    command[3] = flags;
    command[4] = addressSize;
    command[5] = addressesSize;
    command[6] = IPC_PROCESS_ID;
    command[8] = StaticBufferDescriptor(size, 12);
    command[9] = reinterpret_cast<uptr>(buffer);
    command[10] = StaticBufferDescriptor(addressesSize, 13);
    command[11] = reinterpret_cast<uptr>(addresses);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x0048887C | fefates:bytes [tier A]
nn::Result nn::socket::detail::User::Bind(s32* result, s32 socket, const u8* address, size_t addressSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_BIND;
    command[1] = socket;
    command[2] = addressSize;
    command[3] = IPC_PROCESS_ID;
    command[5] = StaticBufferDescriptor(addressSize, 0);
    command[6] = reinterpret_cast<uptr>(address);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x004888DC | fefates:bytes [tier B]
nn::Result nn::socket::detail::User::Poll(s32* result, const nn::socket::PollFd* fds, nn::socket::PollFd* resultFds, u32 count, s32 timeout)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_POLL;
    command[1] = count;
    command[2] = timeout;
    command[3] = IPC_PROCESS_ID;
    size_t size = count * sizeof(nn::socket::PollFd);
    command[5] = StaticBufferDescriptor(size, 10);
    command[6] = reinterpret_cast<uptr>(fds);
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(size, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(resultFds);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x00488968 | fefates:bytes [tier B]
nn::Result nn::socket::detail::User::Close(s32* result, s32 socket)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CLOSE;
    command[1] = socket;
    command[2] = IPC_PROCESS_ID;
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x004889AC | fefates:bytes [tier A]
nn::Result nn::socket::detail::User::Fcntl(s32* result, s32 socket, s32 operation, s32 argument)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_FCNTL;
    command[1] = socket;
    command[2] = operation;
    command[3] = argument;
    command[4] = IPC_PROCESS_ID;
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x004889F8 | fefates:bytes [tier A]
// the data in a mapped buffer
nn::Result nn::socket::detail::User::SendTo(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* address, size_t addressSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SEND_TO;
    command[1] = socket;
    command[2] = size;
    command[3] = flags;
    command[4] = addressSize;
    command[5] = IPC_PROCESS_ID;
    command[7] = StaticBufferDescriptor(addressSize, 1);
    command[8] = reinterpret_cast<uptr>(address);
    command[9] = ReadBufferDescriptor(size);
    command[10] = reinterpret_cast<uptr>(buffer);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x00488A7C | fefates:bytes [tier A]
nn::Result nn::socket::detail::User::Socket(s32* result, s32 domain, s32 type, s32 protocol)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SOCKET;
    command[1] = domain;
    command[2] = type;
    command[3] = protocol;
    command[4] = IPC_PROCESS_ID;
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x00488AC8 | tier C (confirmed by the code)
nn::Result nn::socket::detail::User::Connect(s32* result, s32 socket, const u8* address, size_t addressSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_CONNECT;
    command[1] = socket;
    command[2] = addressSize;
    command[3] = IPC_PROCESS_ID;
    command[5] = StaticBufferDescriptor(addressSize, 0);
    command[6] = reinterpret_cast<uptr>(address);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x00488B28 | fefates:bytes [tier A]
// the data into a mapped buffer, the address into a static buffer
nn::Result nn::socket::detail::User::RecvFrom(s32* result, s32 socket, u8* buffer, s32 size, s32 flags, u8* address, size_t addressSize)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RECV_FROM;
    command[1] = socket;
    command[2] = size;
    command[3] = flags;
    command[4] = addressSize;
    command[5] = IPC_PROCESS_ID;
    command[7] = WriteBufferDescriptor(size);
    command[8] = reinterpret_cast<uptr>(buffer);
    bit32* staticBuffers = nn::os::detail::GetIpcStaticBuffers();
    bit32 saved0 = staticBuffers[0];
    bit32 saved1 = staticBuffers[1];
    staticBuffers[0] = StaticBufferDescriptor(addressSize, 0);
    staticBuffers[1] = reinterpret_cast<uptr>(address);
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    staticBuffers[0] = saved0;
    staticBuffers[1] = saved1;
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x00488BAC | fefates:bytes [tier B]
nn::Result nn::socket::detail::User::Shutdown(s32* result, s32 socket, s32 how)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SHUTDOWN;
    command[1] = socket;
    command[2] = how;
    command[3] = IPC_PROCESS_ID;
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *result = command[2];
    return nn::Result(command[1]);
}

// 0x00488BF0 | fefates:bytes [tier B]
nn::Result nn::socket::detail::User::GetHostId(u32* hostId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_HOST_ID;
    nn::Result ipcResult = nn::svc::SendSyncRequest(mSession);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    *hostId = command[2];
    return nn::Result(command[1]);
}

} // namespace detail
} // namespace socket
} // namespace nn
