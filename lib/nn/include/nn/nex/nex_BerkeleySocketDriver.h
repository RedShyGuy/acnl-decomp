#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"
#include "nn/nex/nex_SocketDriver.h"

#include <cstring>

// The addresses of a SendTo to port 0 (a send to all of them at once). The name is from the
// RTTI of its PseudoGlobalVariable (outside of the namespaces); the names of the members are ours.
struct SendMultiParameter
{
    // an address as the socket service takes it (network byte order)
    struct Address
    {
        u8 m_Length;   // 0x00
        u8 m_Family;   // 0x01
        u16 m_Port;    // 0x02
        u32 m_Address; // 0x04
    };
    static const u32 MAX_ADDRESSES = 15;
    static const u32 LOCK_LEVEL = 0x20000000;

    SendMultiParameter(); // 0x002E6F54
    // (inline) a lock of its own
    SendMultiParameter(const SendMultiParameter& other) : m_Lock(other.m_Lock.m_LockChecker.m_Level), m_Count(other.m_Count)
    {
        memcpy(m_Addresses, other.m_Addresses, sizeof(m_Addresses));
        m_Index = other.m_Index;
    }
    // (inline) the addresses, not the lock
    SendMultiParameter& operator=(const SendMultiParameter& other)
    {
        m_Count = other.m_Count;
        memcpy(m_Addresses, other.m_Addresses, sizeof(m_Addresses));
        m_Index = other.m_Index;
        return *this;
    }

    u32 m_Unknown0x00;
    nn::nex::ForcedCriticalSection m_Lock; // 0x04
    u32 m_Count;                           // 0x2C
    Address m_Addresses[MAX_ADDRESSES];    // 0x30
    u32 m_Index;                           // 0xA8 (the first address of the next send, the sends rotate)
};
ASSERT_OFFSET(SendMultiParameter, m_Count, 0x2C);
ASSERT_SIZE(SendMultiParameter, 0xAC);

namespace nn {
namespace nex {
class InetAddress;

// The sockets of the socket service (nn::socket). The names of the members are ours.
// RTTI N2nn3nex20BerkeleySocketDriverE @ 0x008CE850
// vtable 0x008FD7C0 (vptr 0x008FD7C8), offset_to_top 0, 5 entries
class BerkeleySocketDriver : public ::nn::nex::SocketDriver
{
public:
    class BerkeleySocket;

    // the counts of the calls (name is ours)
    struct Counters
    {
        u32 m_SendToCount;     // 0x00
        u32 m_SendMultiCount;  // 0x04
        u32 m_ReceivedCount;   // 0x08 (RecvFrom)
        u32 m_RecvFromCount;   // 0x0C
    };

    // the functions that see each call (they do nothing by default; the names are ours)
    typedef void (*RecvFromHook)(_SocketResult result, unsigned char* pBuffer, unsigned int size, InetAddress* pAddress, unsigned int* pReceivedSize);
    typedef void (*SendToHook)(_SocketResult result, unsigned char* pBuffer, unsigned int size, const InetAddress* pAddress, unsigned int* pSentSize);
    typedef void (*RecvHook)(_SocketResult result, unsigned char* pBuffer, unsigned int size, unsigned int* pReceivedSize);
    typedef void (*SendHook)(_SocketResult result, unsigned char* pBuffer, unsigned int size, unsigned int* pSentSize);
    static RecvFromHook s_RecvFromHook;
    static SendToHook s_SendToHook;
    static RecvHook s_RecvHook;
    static SendHook s_SendHook;
    static Counters s_Counters;

    // (inline)
    BerkeleySocketDriver() {}
    virtual ~BerkeleySocketDriver(); // 0x00396214 slot 0x00
    // 0x003961FC slot 0x04 (deleting dtor)
    virtual Socket* Create(); // 0x0039615C slot 0x08 | fefates:callseq
    virtual void Delete(nn::nex::SocketDriver::Socket* pSocket); // 0x003961E4 slot 0x0C | fefates:bytes
    virtual bool Poll(nn::nex::SocketDriver::PollInfo* pInfos, unsigned int count, unsigned int timeout); // 0x00395FDC slot 0x10 | fefates:bytes-fuzzy
    // the addresses of the sends to port 0 (none if there are more than maxCount; the name is ours)
    static void GetSendMultiAddresses(nn::nex::InetAddress* pAddresses, unsigned int maxCount, unsigned int* pCount); // 0x00395EE4
};

// the addresses of the sends to port 0 (0x00ADDC54; name is ours)
extern PseudoGlobalVariable< ::SendMultiParameter> g_SendMultiParameter;
} // namespace nex
} // namespace nn
