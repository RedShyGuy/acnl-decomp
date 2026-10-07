#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace nex {
class BackEndServices;
class InetAddress;
class MatchMakingClient;
class NATTraversalRelayClient;
class NgsBridgeInterface;
class NotificationEventHandler;
class StationURL;
template <typename T> class qList;
template <typename T0> class qVector;
} // namespace nex
namespace pia {
namespace common {
class InetAddress;
class SignatureSetting;
} // namespace common
namespace transport {
class StationConnectionInfo;
class StationLocation;
} // namespace transport
namespace inet {
class NatTraverser;
class NexNatRelay;
class Socket;
class SocketInputStream;
class SocketOutputStream;

// RTTI N2nn3pia4inet9NexFacadeE @ 0x008CFAB0
// vtable 0x00900804 (vptr 0x0090080C), offset_to_top 0, 16 entries
//
// The connection of pia to nex (a singleton): the nex objects of the game, the socket and the
// streams of inet, the NAT session (NatTraverser) and the NAT relay. The layout is from
// CreateInstance; the member names are ours.
class NexFacade : public ::nn::pia::common::RootObject
{
public:
    // what the game passes to Bind (the layout is from Bind; the names are ours)
    struct LoginInfo
    {
        nex::BackEndServices* m_pBackEndServices; // 0x0
        u32 m_Unknown0x4;                          // 0x4
    };

    static NexFacade* s_pInstance; // 0x00975A68
    // never set in this game: the NAT session is skipped and the station URL is taken as it is
    // (name is ours)
    static bool s_IsNatSessionSkipped; // 0x00975A64

    static nn::Result CreateInstance(); // 0x0041307C | fefates:bytes [tier B]
    static void DestroyInstance(); // 0x00413168 | fefates:bytes [tier B]

    // (the constructor is inline in CreateInstance)
    NexFacade()
        : m_Unknown0x4(0), m_pNgsBridge(nullptr), m_pMatchMakingClient(nullptr), m_Unknown0x10(0), m_pRelayClientBuffer(nullptr),
          m_pRelayClient(nullptr), m_pNatTraverser(nullptr), m_pSocket(nullptr), m_pOutputStream(nullptr), m_pInputStream(nullptr),
          m_IsStarted(false), m_IsNatSessionStarted(false), m_Unknown0x34(5000), m_Unknown0x38(1500), m_pNotificationEventHandler(nullptr)
    {
    }
    virtual nn::Result Bind(nn::pia::inet::NexFacade::LoginInfo* pLoginInfo); // 0x00413624 slot 0x00 | fefates:bytes
    virtual void Unbind(); // 0x00413694 slot 0x04 | slot vf_0x04 of nn::pia::inet::NexFacade
    virtual nn::Result Startup(nn::nex::MatchMakingClient* pClient); // 0x00413748 slot 0x08 | slot vf_0x08 of nn::pia::inet::NexFacade
    virtual void Cleanup(); // 0x004136A4 slot 0x0C | fefates:bytes
    // (name is ours)
    virtual nn::Result StartNatSession(nn::pia::common::CallContext* pCallContext); // 0x004131B0 slot 0x10
    virtual nn::Result StartNatSessionAsync(); // 0x00413268 slot 0x14 | slot vf_0x14 of nn::pia::inet::NexFacade
    virtual bool IsCompletedStartNatSession(); // 0x004133B0 slot 0x18 | fefates:bytes
    virtual nn::Result GetStartNatSessionResult(); // 0x00413360 slot 0x1C | fefates:bytes
    // (name is ours)
    virtual nn::Result CancelStartNatSession(); // 0x00413388 slot 0x20
    // (armlink placed it in front of CallContext::Reset)
    virtual void StopNatSession(); // 0x004268B8 slot 0x24 | slot vf_0x24 of nn::pia::inet::NexFacade
    virtual void Trace(u64 flag) const; // 0x0072FAD0 slot 0x28
    virtual ~NexFacade(); // 0x0041395C slot 0x2C
    // 0x0041393C slot 0x30 (deleting dtor)
    virtual nn::Result initialize(); // 0x00412FB4 slot 0x34 | fefates:bytes
    virtual void finalize(); // 0x004138B0 slot 0x38 | fefates:bytes
    virtual nn::Result startNatSessionCore(nn::pia::common::CallContext* pCallContext); // 0x004131BC slot 0x3C | fefates:bytes

    nn::Result CreateProtocols(); // 0x0041314C | fefates:bytes [tier B]
    // the socket binds to the port and the streams of inet start with it
    nn::Result CompleteStartNatSession(unsigned short port); // 0x004132B8 | fefates:bytes [tier B]
    // the mapping of the NAT (NatProperty::m_NatMapping; 0 without a NatTraverser)
    u8 GetNatPropertyMapping(); // 0x004132A8 | fefates:callgraph [tier C]
    bool RegisterNexNotificationEventHandler4Pia(nn::nex::NotificationEventHandler* pHandler); // 0x004134A0 | fefates:bytes [tier B]
    bool UnregisterNexNotificationEventHandler4Pia(nn::nex::NotificationEventHandler* pHandler); // 0x00413554 | fefates:bytes [tier B]

    static void ConvertInetAddressToNexInetAddress(const nn::pia::common::InetAddress& address, nn::nex::InetAddress* pNexAddress); // 0x00357848 | fefates:bytes [tier B]
    static void ConvertNexStationUrlToStationLocation(const nn::nex::StationURL& url, nn::pia::transport::StationLocation* pLocation); // 0x004133D4 | fefates:bytes [tier B]
    // the public and the private station URL of a list of one or two
    static nn::Result ConvertNexStationUrlToStationConnectionInfo(const nn::nex::qList<nn::nex::StationURL>& urls, nn::pia::transport::StationConnectionInfo* pInfo); // 0x00413594 | fefates:callgraph [tier C]
    // the session key of a matchmake session into the key buffer of the signature setting
    // (name is ours, after the other Convert* functions)
    static nn::Result ConvertNexSessionKeyToSignatureSetting(const nn::nex::qVector<u8>& key, nn::pia::common::SignatureSetting* pSetting); // 0x004134E0
    static bool IsBehindNat(const nn::pia::transport::StationLocation& location); // 0x0041304C | fefates:callgraph [tier C]
    static bool IsPublic(const nn::pia::transport::StationLocation& location); // 0x004138A0 | fefates:callgraph [tier C]
    // public and not behind a NAT
    static bool IsGlobal(const nn::pia::transport::StationLocation& location); // 0x0041387C | fefates:bytes [tier B]
    static bool IsEdmMapping(const nn::pia::transport::StationLocation& location); // 0x00413058 | fefates:bytes [tier B]
    static bool IsEimMapping(const nn::pia::transport::StationLocation& location); // 0x0041306C | fefates:callgraph [tier C]

    u32 m_Unknown0x4;                                       // 0x04
    nex::NgsBridgeInterface* m_pNgsBridge;                  // 0x08
    nex::MatchMakingClient* m_pMatchMakingClient;          // 0x0C
    u32 m_Unknown0x10;                                      // 0x10
    // the memory of the relay client (placement new in Startup)
    u8* m_pRelayClientBuffer;                               // 0x14
    nex::NATTraversalRelayClient* m_pRelayClient;          // 0x18
    NexNatRelay* m_pNatRelay;                               // 0x1C
    NatTraverser* m_pNatTraverser;                          // 0x20
    Socket* m_pSocket;                                      // 0x24
    SocketOutputStream* m_pOutputStream;                    // 0x28
    SocketInputStream* m_pInputStream;                      // 0x2C
    bool m_IsStarted;                                       // 0x30
    bool m_IsNatSessionStarted;                             // 0x31
    u32 m_Unknown0x34;                                      // 0x34
    u32 m_Unknown0x38;                                      // 0x38
    nex::NotificationEventHandler* m_pNotificationEventHandler; // 0x3C
    common::CallContext m_StartNatSessionCallContext;      // 0x40
};
ASSERT_OFFSET(NexFacade, m_pNatTraverser, 0x20);
ASSERT_OFFSET(NexFacade, m_StartNatSessionCallContext, 0x40);
ASSERT_SIZE(NexFacade, 0x54);
} // namespace inet
} // namespace pia
} // namespace nn
