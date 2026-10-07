#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/nex/nex_ProtocolCallContext.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_InetAddress.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_SimpleContainer.h"
#include "nn/pia/inet/inet_NatPortDetecter.h"
#include "nn/pia/inet/inet_NatProperty.h"
#include "nn/pia/inet/inet_NatPropertyDetecter.h"
#include "nn/pia/inet/inet_NatServerAddressResolveJob.h"
#include "nn/pia/transport/transport_ProtocolId.h"

namespace nn {
namespace nex {
class StationURL;
} // namespace nex
namespace pia {
namespace transport {
class Protocol;
class StationLocation;
} // namespace transport
namespace inet {
class NexNatTraversalProtocol;

// RTTI N2nn3pia4inet12NatTraverserE @ 0x008CF860
// vtable 0x008FFF04 (vptr 0x008FFF0C), offset_to_top 0, 5 entries
//
// The NAT session of inet (NexFacade::m_pNatTraverser): Startup resolves the addresses of the NAT
// check servers, checks the NAT (NatPropertyDetecter, the first time) and the port it maps to
// (NatPortDetecter), and then registers the station URLs with nex. The NAT traversals themselves
// run in NexNatTraversalProtocol. The member names are ours.
class NatTraverser : public ::nn::pia::common::RootObject
{
public:
    // m_State
    enum State : u8
    {
        STATE_NONE = 0,
        STATE_RESOLVING_SERVER_ADDRESS = 1,
        STATE_DETECTING_PROPERTY = 2,
        STATE_DETECTING_PORT = 3,
        STATE_STARTED = 4,
    };

    // the ports of the NAT check servers (name is ours)
    static const u16 NAT_SERVER_PRIMARY_PORT = 10025;
    static const u16 NAT_SERVER_SECONDARY_PORT = 10125;

    NatTraverser(); // 0x003E67D8 | fefates:bytes [tier B]
    virtual ~NatTraverser(); // 0x003E69A8 slot 0x00
    // 0x003E68B4 slot 0x04 (deleting dtor)
    virtual nn::Result Startup(nn::pia::common::CallContext* pCallContext); // 0x003E6714 slot 0x08 | slot vf_0x08 of nn::pia::inet::NatTraverser
    virtual void Cleanup(); // 0x003E6694 slot 0x0C | fefates:bytes
    virtual void Trace(u64 flag) const; // 0x0072EFF0 slot 0x10

    nn::Result CreateProtocols(); // 0x003E526C | fefates:bytes [tier B]
    void stopNatSession(); // 0x003E5124 | fefates:callgraph [tier C]
    // the NAT traversal to the location is no longer needed
    void ClearNatTraversal(const nn::pia::transport::StationLocation& location, bool isConnected); // 0x003E53C0 | fefates:callgraph [tier C]
    bool StartNatTraversal(); // 0x003E53F8 | fefates:bytes [tier B]
    // starts the traversal to the station (isCheckOnly: only whether it is needed); true if it
    // runs (name is ours; armlink placed it in front of NexNatTraversalProtocol::startNatTraversal)
    bool RequestNatTraversal(const nn::pia::transport::StationLocation& location, const nn::pia::transport::StationConnectionInfo& info, nn::pia::common::CallContext* pCallContext, bool isCheckOnly); // 0x00406A00
    bool IsStartupCancelled(); // 0x003E5438 | fefates:bytes [tier B]
    // the addresses of the NAT check servers (from their host names)
    nn::Result UpdateNatServerAddress(); // 0x003E56D4 | fefates:bytes [tier B]
    void ReportNatTraversalResult(const nn::pia::transport::StationLocation& location, nn::Result result, bool isReport); // 0x003E5F98 | fefates:callgraph [tier C]
    // true if the protocol knows a newer location of the station (it is written to location)
    bool CheckLatestStationLocation(nn::pia::transport::StationLocation& location); // 0x003E651C | fefates:bytes [tier B]

    // (names are ours)
    nn::Result startNatServerAddressResolve(); // 0x003E5310
    nn::Result startNatPropertyDetection(); // 0x003E6408
    nn::Result startNatPortDetection(); // 0x003E544C | fefates:callgraph [tier C]
    void OnServerAddressResolved(); // 0x003E65B8
    void OnPortDetected(bool isDetected); // 0x003E62C8
    // the NAT session is ready: the nex station URLs are updated and the call succeeds
    void completeNatSession(); // 0x003E5D0C
    // the private and public station URLs of the console with the NAT and the port
    nn::Result updateStationUrls(); // 0x003E5948
    // the monitoring slot of the station is free again
    void ClearMonitoringNatTraversal(const nn::pia::transport::StationLocation& location); // 0x003E55DC
    // the monitoring slot of the station (or a free one; 255 if there is none)
    u8 FindMonitoringNatTraversalIndex(const nn::pia::transport::StationLocation& location); // 0x003E636C

    // the callbacks of the call contexts (the argument is the traverser)
    static void OnServerAddressResolvedCallback(nn::Result result, void* pArg); // 0x003E65B0
    static void OnPortDetectedCallback(nn::Result result, void* pArg); // 0x003E62BC
    // (armlink placed it far away, next to the inet socket)
    static void OnPropertyDetectedCallback(nn::Result result, void* pArg); // 0x004123A0
    // the completion of the nex call that replaces the station URL (empty)
    static void OnReplaceUrlCompleted(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext); // 0x004110BC

    common::CallContext* m_pCallContext;                                    // 0x004
    NatProperty m_NatProperty;                                              // 0x008
    NatServerAddressResolveJob m_ServerAddressResolveJob;                   // 0x020
    common::SimpleContainer<common::InetAddress, 4> m_ServerAddresses;      // 0x078
    NexNatTraversalProtocol* m_pProtocol;                                   // 0x0A0
    transport::ProtocolId m_ProtocolId;                                     // 0x0A4
    State m_State;                                                          // 0x0A8
    u16 m_LocalPort;                                                        // 0x0AA
    NatPortDetecter m_PortDetecter;                                         // 0x0B0
    NatPropertyDetecter m_PropertyDetecter;                                 // 0x4C8
    common::CallContext m_ServerAddressCallContext;                         // 0x890
    common::CallContext m_PropertyCallContext;                              // 0x8A4
    common::CallContext m_PortCallContext;                                  // 0x8B8
    // the station URLs before and after the update (nex replaces the old one with the new one)
    nex::StationURL* m_pOldStationUrl;                                      // 0x8CC
    nex::StationURL* m_pNewStationUrl;                                      // 0x8D0
    nex::ProtocolCallContext m_ProtocolCallContext;                         // 0x8D8
};
ASSERT_OFFSET(NatTraverser, m_ServerAddressResolveJob, 0x20);
ASSERT_OFFSET(NatTraverser, m_ServerAddresses, 0x78);
ASSERT_OFFSET(NatTraverser, m_pProtocol, 0xA0);
ASSERT_OFFSET(NatTraverser, m_PortDetecter, 0xB0);
ASSERT_OFFSET(NatTraverser, m_PropertyDetecter, 0x4C8);
ASSERT_OFFSET(NatTraverser, m_ServerAddressCallContext, 0x890);
ASSERT_OFFSET(NatTraverser, m_pOldStationUrl, 0x8CC);
ASSERT_OFFSET(NatTraverser, m_ProtocolCallContext, 0x8D8);
ASSERT_SIZE(NatTraverser, 0x950);
} // namespace inet
} // namespace pia
} // namespace nn
