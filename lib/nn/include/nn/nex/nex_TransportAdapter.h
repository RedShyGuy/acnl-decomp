#pragma once

#include "decomp.h"
#include "nn/nex/nex_InetAddress.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qProtectedList.h"

namespace nn {
namespace nex {
class CallContext;
class InterfaceInfo;
class SocketDriver;
class StationURL;
class String;

// The adapter of the transport to the network of the platform: the local URLs, the addresses of the
// interface and the socket driver. The names of the members and of the functions without a
// reference name are ours.
// RTTI N2nn3nex16TransportAdapterE @ 0x008CE62C
// vtable 0x008FD28C (vptr 0x008FD294), offset_to_top 0, 13 entries
class TransportAdapter : public ::nn::nex::RootObject
{
public:
    // (inline)
    TransportAdapter() : m_Unknown0x04(2), m_pSocketDriver(nullptr) {}
    virtual ~TransportAdapter(); // 0x00383F78 slot 0x00
    // 0x00383F54 slot 0x04 (deleting dtor)
    // the URLs of the interfaces of the console (one with the address of the console if none)
    virtual bool GetLocalURLs(nn::nex::qList<nn::nex::StationURL>* pURLs); // 0x00383A4C slot 0x08 | fefates:bytes
    // the address of the URL is already resolved: the call succeeds
    virtual void ResolveAddress(nn::nex::CallContext* pContext, nn::nex::StationURL* pURL); // 0x00383D90 slot 0x0C | fefates:callseq
    virtual void ReleaseAddress(const nn::nex::StationURL& url); // 0x00383D8C slot 0x10
    // the URLs of the target to contact, in order (see StationContactInfo)
    virtual void SortAndFilterTargetURLs(const nn::nex::qProtectedList<nn::nex::StationURL>& localURLs, nn::nex::qList<nn::nex::StationURL>* pTargetURLs); // 0x00383E40 slot 0x14 | fefates:callseq
    virtual bool IsReady(); // 0x00383DD8 slot 0x18 (name is ours)
    // the address of the URL (host byte order)
    virtual u32 GetAddress(const nn::nex::StationURL& url); // 0x0072B778 slot 0x1C (name is ours)
    virtual void Load(); // 0x00383F4C slot 0x20
    virtual void Unload(); // 0x00383F50 slot 0x24
    virtual bool SetNetworkInterfaceInfo(nn::nex::InterfaceInfo* pInfo); // 0x00383DF8 slot 0x28 | fefates:bytes
    virtual bool SetLocalAddress(const nn::nex::String& address); // 0x00383DE0 slot 0x2C | mk7dlp:bytes
    virtual bool SetPortNumber(unsigned short port); // 0x00383D7C slot 0x30

    // the adapter of the transport
    static TransportAdapter* s_pInstance;
    // the adapter if the platform sets none (name is ours)
    static TransportAdapter s_DefaultInstance;

    u8 m_Unknown0x04;               // 0x04
    InetAddress m_BroadcastAddress; // 0x08 (the broadcast address of the interface)
    InetAddress m_Unknown0x28;      // 0x28 (0.0.0.0 with the interface info)
    InetAddress m_LocalAddress;     // 0x48 (the address of the interface and the port)
    SocketDriver* m_pSocketDriver;  // 0x68 (the driver of the platform; the default one if none)
};
ASSERT_OFFSET(TransportAdapter, m_LocalAddress, 0x48);
ASSERT_SIZE(TransportAdapter, 0x6C);
} // namespace nex
} // namespace nn
