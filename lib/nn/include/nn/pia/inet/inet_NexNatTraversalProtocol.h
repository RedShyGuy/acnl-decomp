#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/inet/inet_NatProbeList.h"
#include "nn/pia/inet/inet_NatProbeRequestList.h"
#include "nn/pia/inet/inet_NatTraversalTimeList.h"
#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace inet {
class NatPortDetecter;
class NatRelayInterface;

// RTTI N2nn3pia4inet23NexNatTraversalProtocolE @ 0x008CF9CC
// vtable 0x009004F8 (vptr 0x00900500), offset_to_top 0, 9 entries
//
// The NAT traversal between the stations: probes, the probe requests through the relay of the nex
// server, the keep-alive to the NAT check server and the times of the traversals. The layout is
// from the constructor; the member names are ours.
class NexNatTraversalProtocol : public ::nn::pia::transport::Protocol
{
public:
    NexNatTraversalProtocol(); // 0x00407808 | fefates:bytes [tier B]
    virtual ~NexNatTraversalProtocol(); // 0x00407AF0 slot 0x00 | fefates:bytes
    // 0x00407AE0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072EFE4 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual u16 GetProtocolType() const; // 0x0072F530 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
    virtual nn::Result Dispatch(); // 0x00407374 slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
    virtual bool IsEnableProtocolFiltering() const; // 0x0072F538 slot 0x20 | slot vf_0x20 of nn::pia::transport::Protocol

    // the probe of the station (name is ours; armlink placed it in NatProbeList)
    NatProbe* findProbe(const nn::pia::transport::StationLocation& location); // 0x003E4788
    // a debug flag: the probes go to the port + 10 (name is ours)
    static bool* GetProbePortShiftFlag(); // 0x003E70BC

    // sends the probes that are due
    void sendProbes(); // 0x00405044 | fefates:callgraph [tier C]
    // a probe of the location got a reply (its timeout starts again)
    bool IsTraversed(const nn::pia::transport::StationLocation& location); // 0x004054A4 | fefates:bytes [tier B]
    bool IsTraversed(unsigned int stationKey); // 0x00405570 | fefates:bytes [tier B]
    // false if a relay is registered already
    bool RegisterRelay(nn::pia::inet::NatRelayInterface* pRelay); // 0x004055B0 | fefates:bytes [tier B]
    // the requests, the probes and the probe requests that are due (name is ours)
    nn::Result updateNatTraversal(const nn::pia::common::Time& now); // 0x0040563C
    bool UnregisterRelay(); // 0x00405A64 | fefates:bytes [tier B]
    void addProbeRequest(const nn::pia::inet::NatProbeRequest& request); // 0x00405A98 | fefates:bytes [tier B]
    // num "Dummy" messages to open the NAT for the location
    void sendDummyPacket(const nn::pia::transport::StationLocation& location, unsigned char ttl, unsigned char num); // 0x00405C54 | fefates:bytes [tier B]
    void ResetProbeStatus(const nn::pia::transport::StationLocation& location, bool isAddressOnly); // 0x00405D54 | fefates:bytes [tier B]
    // through the relay
    void SendProbeRequest(const nn::pia::transport::StationLocation& target, const nn::pia::transport::StationLocation& self); // 0x00405E3C | fefates:bytes [tier B]
    // the port the station uses for direct (peer to peer) traffic, 0 if it is not known
    u16 getPeer2PeerPort(const nn::pia::transport::StationLocation& location); // 0x00405E64 | fefates:bytes [tier B]
    void sendProbeRequest(nn::pia::transport::StationLocation self, const nn::pia::transport::StationLocation& target); // 0x00406098 | fefates:callgraph [tier C]
    // the traversal to the location is no longer needed (name is ours)
    void ClearNatTraversal(const nn::pia::transport::StationLocation& location, bool isConnected); // 0x00406378
    // false if no request is left
    bool sendProbeRequests(); // 0x00406508 | fefates:bytes [tier B]
    void CleanupNatTraversal(); // 0x004067F0 | fefates:bytes-fuzzy [tier B]
    // (name is ours) a traversal with a probe request to the station; false if there is a probe
    // with a reply already
    bool startNatTraversal(const nn::pia::transport::StationLocation& location, const nn::pia::transport::StationConnectionInfo& info, nn::pia::common::CallContext* pCallContext); // 0x00406A40
    // probes to the location (the relay asked for them; name is ours)
    void StartProbe(const nn::pia::transport::StationLocation& location); // 0x00406C44
    void ReportNatProperties(const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt); // 0x00406F28 | fefates:bytes [tier B]
    nn::Result StartupNatTraversal(nn::pia::inet::NatPortDetecter* pPortDetecter); // 0x00406F44 | fefates:callgraph [tier C]
    // (name is ours)
    void StopServerKeepAlive(); // 0x00407044
    void StartServerKeepAlive(); // 0x00407050 | fefates:bytes [tier B]
    void scheduleProbeRequest(const nn::pia::inet::NatProbeRequest& request); // 0x00407090 | fefates:bytes [tier B]
    // true if a probe knows another location of the station (it is written to pLatest)
    bool GetLatestStationLocation(const nn::pia::transport::StationLocation& location, nn::pia::transport::StationLocation* pLatest); // 0x0040716C | fefates:bytes [tier B]
    void ReportNatTraversalResult(const nn::pia::transport::StationLocation& location, bool isSucceeded); // 0x004071BC | fefates:bytes [tier B]
    // (name after NatRelayInterface::SetLocalCID, which calls it)
    void SetLocalCid(unsigned int cid); // 0x0040722C
    NatProbeRequest* findProbeRequestByConnectionId(unsigned int stationKey); // 0x00407240 | fefates:bytes [tier B]
    // true if one was erased
    bool eraseProbeRequestByConnectionId(unsigned int stationKey); // 0x0040727C | fefates:bytes [tier B]
    // a probe (type 0) or a reply (type 1) to the location (name is ours)
    void sendProbe(u8 type, const nn::pia::transport::StationLocation& location, const nn::pia::common::Time& sendTime, u8 ttl); // 0x00407680
    // the callback of the port check of an inverse traversal (name is ours; armlink placed it
    // next to the inet socket)
    static void OnPortDetectedCallback(nn::Result result, void* pArg); // 0x00412318

    bool m_IsStarted;                                    // 0x014
    // the station key of the console (0: no probes yet)
    u32 m_LocalCid;                                      // 0x018
    // the location of the console (NatTraverser::completeNatSession)
    transport::StationLocation m_SelfLocation;           // 0x01C
    // the keep-alive server (the primary NAT check server, port 33335)
    transport::StationLocation m_ServerLocation;         // 0x044
    NatRelayInterface* m_pRelay;                         // 0x06C
    // of the current probe request
    common::CallContext m_CallContext;                   // 0x070
    NatProbeList m_ProbeList;                            // 0x084
    NatProbeRequestList m_ProbeRequestList;              // 0x0B8
    // requests that wait for an inverse traversal
    NatProbeRequestList m_ScheduledProbeRequestList;     // 0x0EC
    NatTraversalTimeList m_TraversalTimeList;            // 0x120
    common::Time m_NextUpdateTime;                       // 0x4B0
    common::Time m_RequestDeadline;                      // 0x4B8
    bool m_IsRequesting;                                 // 0x4C0
    bool m_IsInverseRequest;                             // 0x4C1
    bool m_IsDirectRequest;                              // 0x4C2
    bool m_Unknown0x4C3;                                 // 0x4C3
    bool m_IsPortUnknown;                                // 0x4C4
    bool m_Unknown0x4C5;                                 // 0x4C5
    u32 m_RequestStationKey;                             // 0x4C8
    transport::StationLocation m_RequestLocation;        // 0x4CC
    bool m_IsServerKeepAlive;                            // 0x4F4
    common::Time m_ServerKeepAliveTime;                  // 0x4F8
    NatPortDetecter* m_pPortDetecter;                    // 0x500
};
ASSERT_OFFSET(NexNatTraversalProtocol, m_SelfLocation, 0x1C);
ASSERT_OFFSET(NexNatTraversalProtocol, m_pRelay, 0x6C);
ASSERT_OFFSET(NexNatTraversalProtocol, m_ProbeList, 0x84);
ASSERT_OFFSET(NexNatTraversalProtocol, m_TraversalTimeList, 0x120);
ASSERT_OFFSET(NexNatTraversalProtocol, m_RequestLocation, 0x4CC);
ASSERT_OFFSET(NexNatTraversalProtocol, m_pPortDetecter, 0x500);
ASSERT_SIZE(NexNatTraversalProtocol, 0x508);
} // namespace inet
} // namespace pia
} // namespace nn
