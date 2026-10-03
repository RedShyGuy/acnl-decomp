#include "nn/socket/detail/detail_Api.h"
#include "nn/socket/detail/socket_DnsUserClient.h"
#include "nn/os/os_IpcSession.h"
#include "nn/os/os_TransferMemoryBlock.h"
#include "nn/socket/detail/socket_SessionPoolAuto.h"
#include "nn/socket/detail/socket_User.h"
#include "nn/socket/socket_Privileged.h"
#include "nn/srv/srv_Api.h"

#include <new>

namespace nn {
namespace socket {
namespace detail {

namespace {
const char SERVICE_NAME[] = "soc:U";

// the transfer memory: the service may read and write it
const u32 TRANSFER_MEMORY_MY_PERMISSION = 0;
const u32 TRANSFER_MEMORY_OTHER_PERMISSION = 3;
} // namespace

// the globals of socket_IpcWrapper.cpp (names are ours)
// 0x0097F070
DnsUserClient* s_pDnsClient;
// 0x00AEE7C4
nn::os::TransferMemoryBlock s_TransferMemory;
// 0x0097F074
bool s_IsControlSessionInitialized;
// 0x0097F078
nn::os::ipc::Session s_ControlSession;
// 0x00AEE7E4
SessionPoolAuto s_SessionPool;
// [0]: a privileged session (GetHostId uses it if it is open)
// 0x0097F07C
nn::os::ipc::Session s_PrivilegedSessions[2];

namespace {

// a session of the pool for one call; given back by the destructor (name is ours)
class ScopedSession
{
public:
    ScopedSession() : mItem(0) {}
    ~ScopedSession()
    {
        if (mItem != 0) {
            s_SessionPool.Release(mItem);
        }
    }

    nn::Result Acquire() { return s_SessionPool.Acquire(&mItem); }

    // waits while all sessions are busy (for the calls that must not fail for that)
    nn::Result AcquireWaiting()
    {
        for (;;) {
            nn::Result result = Acquire();
            if (result.IsFailure() && result.GetDescription() == DESCRIPTION_NO_FREE_SESSION) {
                s_SessionPool.WaitForRelease();
                continue;
            }
            return result;
        }
    }

    User GetUser() const { return User(mItem->mSession.GetHandle()); }

private:
    SessionItem* mItem;
};

// the control session (inline)
User GetControlUser()
{
    return User(s_ControlSession.GetHandle());
}

} // namespace

// 0x00484E74 | fefates:bytes [tier B]
nn::Result SetSockOpt(s32* result, s32 socket, s32 level, s32 name, const u8* value, s32 valueSize)
{
    if (!s_IsControlSessionInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result ipcResult = GetControlUser().SetSockOpt(result, socket, level, name, value, valueSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x00484EE0 | fefates:bytes [tier B]
nn::Result GetAddrInfo(s32* result, const char* node, size_t nodeSize, const char* service, size_t serviceSize, const u8* hints, s32 hintsSize, s32* count, u8* addrInfo, size_t addrInfoSize)
{
    ScopedSession session;
    nn::Result ipcResult = session.Acquire();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().GetAddrInfo(result, node, nodeSize, service, serviceSize, hints, hintsSize, count, addrInfo, addrInfoSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x00485378 (name is ours, after User::GetSockName)
nn::Result GetSockName(s32* result, s32 socket, u8* address, size_t addressSize)
{
    if (!s_IsControlSessionInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result ipcResult = GetControlUser().GetSockName(result, socket, address, addressSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x004853D4 | tier C (confirmed by the code)
nn::Result SendToSmall(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* address, size_t addressSize)
{
    ScopedSession session;
    nn::Result ipcResult = session.Acquire();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().SendToSmall(result, socket, buffer, size, flags, address, addressSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x004862F8 | fefates:bytes [tier B]
nn::Result GetHostByName(s32* result, const char* name, size_t nameSize, u8* hostEntry, size_t hostEntrySize)
{
    ScopedSession session;
    nn::Result ipcResult = session.Acquire();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().GetHostByName(result, name, nameSize, hostEntry, hostEntrySize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x00486780 | fefates:bytes [tier B]
nn::Result GetNetworkOpt(s32* result, s32 level, s32 name, u8* value, s32* valueSize)
{
    if (!s_IsControlSessionInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result ipcResult = GetControlUser().GetNetworkOpt(result, level, name, value, valueSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x004867E4 | fefates:bytes [tier B]
nn::Result RecvFromSmall(s32* result, s32 socket, u8* buffer, s32 size, s32 flags, u8* address, size_t addressSize)
{
    ScopedSession session;
    nn::Result ipcResult = session.Acquire();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().RecvFromSmall(result, socket, buffer, &size, flags, address, addressSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x00486DCC | fefates:bytes [tier B]
nn::Result SendToSmallMulti(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* addresses, size_t addressSize, size_t addressesSize)
{
    ScopedSession session;
    nn::Result ipcResult = session.Acquire();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().SendToSmallMulti(result, socket, buffer, size, flags, addresses, addressSize, addressesSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x00487284 | fefates:bytes-fuzzy [tier B]
// the negative error numbers are the library's (values from the binary)
s32 ConvertErrorResult(nn::Result result)
{
    if (result.IsSuccess()) {
        return 0;
    }
    bit32 module = result.GetModule();
    bit32 description = result.GetDescription();
    if (module == 6 && description == 26) {
        return -39;
    }
    switch (description) {
    case 7:
        return -8;
    case 8:
        return -10;
    case 1000:
    case 1004:
    case 1005:
    case 1006:
    case 1009:
    case 1010:
    case 1012:
    case 1013:
    case 1014:
    case 1015:
        return -28;
    case 1002:
        return -2;
    case 1007:
        return -43;
    case 1008:
        return -10;
    case 1011:
        return -49;
    case 1016:
        return -39;
    case 1017:
        return -7;
    default:
        return -28;
    }
}

// 0x00487390 | fefates:bytes [tier B]
void FinalizeSessionPool()
{
    s_SessionPool.Finalize();
}

// 0x0048746C | fefates:bytes [tier B]
nn::Result InitializeDnsClient()
{
    void* buffer = s_Heap.Allocate(sizeof(DnsUserClient), 4);
    nn::Result result = (buffer == 0) ? nn::Result(SessionPoolAuto::RESULT_OUT_OF_MEMORY) : nn::Result();
    if (result.IsFailure()) {
        return result;
    }
    s_pDnsClient = new (buffer) DnsUserClient;
    return nn::Result();
}

// 0x00487528 | fefates:bytes [tier B]
nn::Result InitializeSessionPool(nn::fnd::IAllocator& allocator, s32 count)
{
    return s_SessionPool.Initialize(allocator, count, SERVICE_NAME);
}

// 0x00487960 | fefates:bytes [tier B]
void FinalizeControlSession()
{
    s_IsControlSessionInitialized = false;
    GetControlUser().DetachProcess();
    s_ControlSession.Close();
    s_TransferMemory.Finalize();
}

// 0x004879B0 | fefates:bytes [tier B]
nn::Result InitializeControlSession(uptr address, size_t size)
{
    nn::Result result = nn::srv::GetServiceHandle(&s_ControlSession, SERVICE_NAME);
    if (result.IsFailure()) {
        return result;
    }
    User user = GetControlUser();
    if (size != 0) {
        s_TransferMemory.Initialize(reinterpret_cast<void*>(address), size, TRANSFER_MEMORY_MY_PERMISSION,
                                    TRANSFER_MEMORY_OTHER_PERMISSION);
    }
    result = user.AttachProcess(s_TransferMemory.GetHandle(), size);
    if (result.IsFailure()) {
        s_ControlSession.Close();
        s_TransferMemory.Finalize();
        return result;
    }
    s_IsControlSessionInitialized = true;
    return nn::Result();
}

// 0x00487A78 | mk7dlp:callgraph [tier A]
size_t GetRequiredMemorySizeForSessionPool(s32 count)
{
    return SessionPoolAuto::GetRequiredMemorySize(count);
}

// 0x00487A84 | tier C (confirmed by the code)
nn::Result Bind(s32* result, s32 socket, const u8* address, size_t addressSize)
{
    ScopedSession session;
    nn::Result ipcResult = session.Acquire();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().Bind(result, socket, address, addressSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x00487F14 | fefates:bytes [tier B]
void Free(void* p)
{
    s_Heap.Free(p);
}

// 0x00487F48 | fefates:bytes [tier B]
nn::Result Poll(s32* result, nn::socket::PollFd* fds, u32 count, s32 timeout)
{
    ScopedSession session;
    nn::Result ipcResult = session.Acquire();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().Poll(result, fds, fds, count, timeout);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x00488C24 | fefates:bytes [tier B]
nn::Result Close(s32* result, s32 socket)
{
    ScopedSession session;
    nn::Result ipcResult = session.AcquireWaiting();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().Close(result, socket);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x004890D0 (name is ours, after User::Fcntl)
nn::Result Fcntl(s32* result, s32 socket, s32 operation, s32 argument)
{
    if (!s_IsControlSessionInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result ipcResult = GetControlUser().Fcntl(result, socket, operation, argument);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x0048912C | tier C (confirmed by the code)
nn::Result SendTo(s32* result, s32 socket, const u8* buffer, s32 size, s32 flags, const u8* address, size_t addressSize)
{
    ScopedSession session;
    nn::Result ipcResult = session.Acquire();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().SendTo(result, socket, buffer, size, flags, address, addressSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x004895CC | tier C (confirmed by the code)
nn::Result Socket(s32* result, s32 domain, s32 type, s32 protocol)
{
    if (!s_IsControlSessionInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result ipcResult = GetControlUser().Socket(result, domain, type, protocol);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x00489628 (name is ours, after User::Connect)
nn::Result Connect(s32* result, s32 socket, const u8* address, size_t addressSize)
{
    ScopedSession session;
    nn::Result ipcResult = session.Acquire();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().Connect(result, socket, address, addressSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x00489AB8 | fefates:bytes [tier B]
void* Allocate(size_t size, s32 alignment)
{
    return s_Heap.Allocate(size, alignment);
}

// 0x00489B18 | tier C (confirmed by the code)
nn::Result RecvFrom(s32* result, s32 socket, u8* buffer, s32 size, s32 flags, u8* address, size_t addressSize)
{
    ScopedSession session;
    nn::Result ipcResult = session.Acquire();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().RecvFrom(result, socket, buffer, size, flags, address, addressSize);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x00489FB8 | fefates:bytes [tier B]
nn::Result Shutdown(s32* result, s32 socket, s32 how)
{
    ScopedSession session;
    nn::Result ipcResult = session.AcquireWaiting();
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    ipcResult = session.GetUser().Shutdown(result, socket, how);
    if (ipcResult.IsFailure()) {
        return ipcResult;
    }
    return nn::Result();
}

// 0x0048A4EC | fefates:bytes [tier B]
nn::Result GetHostId(u32* hostId)
{
    if (s_PrivilegedSessions[0].GetHandle().IsValid()) {
        return Privileged(s_PrivilegedSessions[0].GetHandle()).GetHostId(hostId);
    }
    if (!s_IsControlSessionInitialized) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    nn::Result result = GetControlUser().GetHostId(hostId);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

} // namespace detail
} // namespace socket
} // namespace nn
