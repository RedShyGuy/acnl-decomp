#include "nn/nex/nex_BerkeleySocketDriver.h"
#include "nn/nex/nex_BerkeleySocketDriver_BerkeleySocket.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_MemoryManager.h"
#include "nn/socket/detail/detail_Api.h"
#include "nn/socket/socket_Types.h"

#include <cstring>
#include <new>

// 0x002E6F54
SendMultiParameter::SendMultiParameter() : m_Lock(LOCK_LEVEL), m_Count(0), m_Index(0)
{
    memset(m_Addresses, 0, sizeof(m_Addresses));
}

namespace nn {
namespace nex {
namespace {
// the events of the socket service
const s32 POLL_READ = 3;
const s32 POLL_WRITE = 8;
// the sockets of one Poll
const u32 MAX_POLL_SOCKETS = 8;

// the hooks of the calls by default (the names are ours)
// 0x0052919C (name is ours)
void DefaultRecvHook(SocketDriver::_SocketResult result, unsigned char* pBuffer, unsigned int size, unsigned int* pReceivedSize)
{
    // nothing to do
}

// 0x005291A0 (name is ours)
void DefaultSendHook(SocketDriver::_SocketResult result, unsigned char* pBuffer, unsigned int size, unsigned int* pSentSize)
{
    // nothing to do
}

// 0x005291A4 (name is ours)
void DefaultSendToHook(SocketDriver::_SocketResult result, unsigned char* pBuffer, unsigned int size, const SocketDriver::InetAddress* pAddress,
                       unsigned int* pSentSize)
{
    // nothing to do
}

// 0x005291A8 (name is ours)
void DefaultRecvFromHook(SocketDriver::_SocketResult result, unsigned char* pBuffer, unsigned int size, SocketDriver::InetAddress* pAddress,
                         unsigned int* pReceivedSize)
{
    // nothing to do
}

// (inline) the return value of a call, or the error number of the IPC result
inline s32 ToReturnValue(nn::Result result, s32 value)
{
    return result.IsSuccess() ? value : nn::socket::detail::ConvertErrorResult(result);
}
} // namespace

// 0x0096CA48 (name is ours)
BerkeleySocketDriver::RecvFromHook BerkeleySocketDriver::s_RecvFromHook = DefaultRecvFromHook;
// 0x0096CA4C (name is ours)
BerkeleySocketDriver::SendToHook BerkeleySocketDriver::s_SendToHook = DefaultSendToHook;
// 0x0096CA50 (name is ours)
BerkeleySocketDriver::RecvHook BerkeleySocketDriver::s_RecvHook = DefaultRecvHook;
// 0x0096CA54 (name is ours)
BerkeleySocketDriver::SendHook BerkeleySocketDriver::s_SendHook = DefaultSendHook;
// 0x00ADDC44 (name is ours)
BerkeleySocketDriver::Counters BerkeleySocketDriver::s_Counters;
// 0x00ADDC54 (name is ours)
PseudoGlobalVariable< ::SendMultiParameter> g_SendMultiParameter;
// 0x0096CA8C (name is ours)
BerkeleySocketDriver g_BerkeleySocketDriver;
// 0x0096C984 (name is ours)
SocketDriver* SocketDriver::s_pDefaultInstance = &g_BerkeleySocketDriver;
// 0x0096CA9C (name is ours)
SocketDriver* SocketDriver::s_pInstance = SocketDriver::s_pDefaultInstance;

// 0x00395EE4 (name is ours)
void nn::nex::BerkeleySocketDriver::GetSendMultiAddresses(nn::nex::InetAddress* pAddresses, unsigned int maxCount, unsigned int* pCount)
{
    ::SendMultiParameter& parameter = g_SendMultiParameter.Get();
    if (parameter.m_Count > maxCount) {
        *pCount = 0;
        return;
    }
    ScopedCS lock(parameter.m_Lock);
    for (u32 i = 0; i < parameter.m_Count; i++) {
        const ::SendMultiParameter::Address& address = parameter.m_Addresses[i % parameter.m_Count];
        pAddresses[i].m_SockAddr.m_Address = __builtin_bswap32(__builtin_bswap32(address.m_Address));
        pAddresses[i].m_SockAddr.m_Port = __builtin_bswap16(__builtin_bswap16(address.m_Port));
    }
    *pCount = parameter.m_Count;
}

// 0x00395FDC slot 0x10 | fefates:bytes-fuzzy
bool nn::nex::BerkeleySocketDriver::Poll(nn::nex::SocketDriver::PollInfo* pInfos, unsigned int count, unsigned int timeout)
{
    nn::socket::PollFd fds[MAX_POLL_SOCKETS];
    memset(fds, 0, sizeof(fds));
    for (u32 i = 0; i < count; i++) {
        fds[i].fd = static_cast<BerkeleySocket*>(pInfos[i].m_pSocket)->m_Socket;
        fds[i].revents = 0;
        pInfos[i].m_ReturnedEvents = 0;
        if (pInfos[i].m_Events & EVENT_READ) {
            fds[i].events = POLL_READ;
        }
        if (pInfos[i].m_Events & EVENT_WRITE) {
            fds[i].events = POLL_WRITE;
        }
    }
    s32 value = 0;
    if (ToReturnValue(nn::socket::detail::Poll(&value, fds, count, timeout), value) <= 0) {
        return false;
    }
    for (u32 i = 0; i < count; i++) {
        if (fds[i].revents & POLL_READ) {
            pInfos[i].m_ReturnedEvents |= EVENT_READ;
        }
        if (fds[i].revents & POLL_WRITE) {
            pInfos[i].m_ReturnedEvents |= EVENT_WRITE;
        }
    }
    return true;
}

// 0x0039615C slot 0x08 | fefates:callseq
nn::nex::SocketDriver::Socket* nn::nex::BerkeleySocketDriver::Create()
{
    void* pMemory = MemoryManager::Allocate(sizeof(BerkeleySocket));
    if (pMemory == nullptr) {
        return nullptr;
    }
    return ::new (pMemory) BerkeleySocket(&g_SendMultiParameter.Get());
}

// 0x003961E4 slot 0x0C | fefates:bytes
void nn::nex::BerkeleySocketDriver::Delete(nn::nex::SocketDriver::Socket* pSocket)
{
    if (pSocket != nullptr) {
        delete pSocket;
    }
}

// 0x00396214 slot 0x00
// 0x003961FC (deleting dtor)
nn::nex::BerkeleySocketDriver::~BerkeleySocketDriver()
{
    // nothing to do
}

} // namespace nex
} // namespace nn
