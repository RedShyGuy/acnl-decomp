#pragma once

#include "decomp.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StationURL.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_URLProbeList.h"
#include "nn/nex/nex_qList.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class InetAddress;
class JobCheckAndRelay;
class NATRelayInterface;
class NATTraversalStream;

// The sender of the probes if one is set (no class of the binary sets one; the name is ours).
class NATProbeSender : public ::nn::nex::RootObject
{
public:
    virtual ~NATProbeSender();
    virtual void vf_0x08();
    // the data goes to the station of the URL
    virtual void SendTo(const nn::nex::StationURL* pURL, const u8* pData, unsigned int size, unsigned char ttl); // slot 0x0C
};
} // namespace nex
} // namespace nn

namespace __rw {
// (the original calls erase(first, last) of the probe requests out of line; defined in
// nex_NATTraversalEngine.cpp)
template <>
DECOMP_NOINLINE __rb_tree<unsigned int, std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >,
                          __select1st<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >, unsigned int>, std::less<unsigned int>,
                          nn::nex::MemAllocator<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> > > >::iterator
__rb_tree<unsigned int, std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >,
          __select1st<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> >, unsigned int>, std::less<unsigned int>,
          nn::nex::MemAllocator<std::pair<const unsigned int, std::pair<nn::nex::Time, nn::nex::StationURL> > > >::erase(iterator first, iterator last);
} // namespace __rw

namespace nn {
namespace nex {
// The NAT traversal between the stations: probes (a ByteStream of the type, the RVCID of the
// console and the send time; a reply adds the port it came from) through the NATTraversalStream,
// the probe requests to the peers through the NATRelayInterface, the relay of the stations whose
// NAT maps the ports per destination and the keep-alive of the NAT check server.
// pia::inet::NexNatTraversalProtocol is a copy of it; the names of the members and of the functions
// without a reference name are ours (after the pia ones where the code is the same).
// RTTI N2nn3nex18NATTraversalEngineE @ 0x008CE724
// vtable 0x008FD51C (vptr 0x008FD524), offset_to_top 0, 8 entries
class NATTraversalEngine : public ::nn::nex::RootObject
{
public:
    typedef qMap<u32, std::pair<Time, StationURL> > ProbeRequestMap;

    NATTraversalEngine(); // 0x0038FE80 | fefates:bytes [tier B]
    virtual ~NATTraversalEngine(); // 0x0039006C slot 0x00 | fefates:bytes
    // 0x0039003C slot 0x04 (deleting dtor)
    // the probes start (with the stream); false if a relay interface is registered already
    virtual bool RegisterRelay(nn::nex::NATRelayInterface* pInterface); // 0x0038D5B0 slot 0x08
    // the stream is deleted; false if no relay interface is registered
    virtual bool UnregisterRelay(); // 0x0038E7F4 slot 0x0C
    // false if one is registered already
    virtual bool RegisterUnknown(void* pUnknown); // 0x0038D484 slot 0x10
    virtual bool UnregisterUnknown(); // 0x0038E1B0 slot 0x14
    // sets ConnectivityManager::m_Unknown0x18 if it is not set
    virtual bool SetConnectivityUnknown(unsigned int value); // 0x0038FB78 slot 0x18
    virtual bool ClearConnectivityUnknown(); // 0x0038FBC0 slot 0x1C

    // resends the probes that are due
    void ResendProbes(); // 0x0038C5E8
    // the probes that got a reply or timed out go to the finished ones (with probe requests to the
    // relay for the URLs of the stations behind the same NAT)
    void ProcessFinishedProbes(); // 0x0038C770
    // true (and the URL in pUpdated) if the reply of the probe came from another URL
    bool GetUpdatedURL(const nn::nex::StationURL& url, nn::nex::StationURL* pUpdated); // 0x0038D4F0 | fefates:bytes [tier B]
    // true if the probe of the URL failed or there is none (nothing waits for one)
    bool IsProbeFailed(const nn::nex::StationURL& url); // 0x0038D6C8
    // a probe (type 0) or a reply (type 1) from the URL
    void ProcessProbe(const nn::nex::StationURL& url, const u8* pData, unsigned int size); // 0x0038D998
    void StartSendProbe(const nn::nex::StationURL& url); // 0x0038DE30 | fefates:bytes-fuzzy [tier B]
    // "Dummy" packets to the URL (on the port of the NAT check server) to open the NAT
    void SendDummyPacket(const nn::nex::StationURL& url, unsigned char ttl); // 0x0038E220
    // a probe to the relay server of the address
    void SendRelayProbe(const nn::nex::InetAddress* pAddress); // 0x0038E420
    // a "Dummy" packet to the first NAT check server
    void SendServerKeepAlive(bool unknown); // 0x0038E584
    // the probe requests to the peers for the URLs of the traversals that start
    void ProcessPendingURLs(); // 0x0038E880
    // the URL waits for a traversal; false if there is one already
    bool PrepareNATTraversal(nn::nex::StationURL url); // 0x0038F5C0 | fefates:callgraph [tier C]
    // the probe requests without a probe of the peer until now go to the finished probes
    void ProcessExpiredProbeRequests(const nn::nex::Time& now); // 0x0038F7EC
    nn::nex::NATTraversalResult GetNATTraversalResult(unsigned int rvcid); // 0x0038FA28 | fefates:callgraph [tier C]
    // runs with NATTraversalStream::DoWork
    void DoWork(); // 0x0038FC08
    // a probe (type 0) or a reply (type 1) to the URL
    void SendProbe(unsigned char type, const nn::nex::StationURL* pURL, const nn::nex::Time* pSendTime, unsigned char ttl); // 0x0038FD30
    // (out of line in the original, placed with the inline copies) the URL goes through the relay
    // server (checkURL: only a URL without a relay whose station is behind a NAT)
    bool ShouldUseRelay(const nn::nex::StationURL& url, bool checkURL) const; // 0x0072C200 | fefates:callgraph [tier C]

    u32 m_Unknown0x04;                     // 0x004
    NATRelayInterface* m_pRelayInterface;  // 0x008
    void* m_pUnknown0x0C;                  // 0x00C
    NATTraversalStream* m_pStream;         // 0x010
    u32 m_LocalCid;                        // 0x014 (the RVCID of the probes)
    NATProbeSender* m_pSender;             // 0x018 (null: the stream sends the probes)
    qList<StationURL> m_PendingURLs;       // 0x01C (the URLs that wait for a traversal)
    URLProbeList m_Probes;                 // 0x034 (the probes that are sent)
    URLProbeList m_FinishedProbes;         // 0x050 (the probes with a reply or without one in time)
    Time m_NextUpdateTime;                 // 0x070
    JobCheckAndRelay* m_pCheckAndRelayJob; // 0x078
    CallContext m_CallContext;             // 0x080 (of the JobCheckAndRelay)
    bool m_IsServerKeepAlive;              // 0x0E0
    Time m_ServerKeepAliveTime;            // 0x0E8
    u32 m_NextPRID;                        // 0x0F0
    ProbeRequestMap m_ProbeRequests;       // 0x0F4 (by PRID: the deadline and the URL)
    String m_RelayHost;                    // 0x110 (Relay::Clear empties it)
    u16 m_RelayPort;                       // 0x118
    bool m_UsesRelay;                      // 0x11A
};
ASSERT_OFFSET(NATTraversalEngine, m_Probes, 0x34);
ASSERT_OFFSET(NATTraversalEngine, m_NextUpdateTime, 0x70);
ASSERT_OFFSET(NATTraversalEngine, m_CallContext, 0x80);
ASSERT_OFFSET(NATTraversalEngine, m_ServerKeepAliveTime, 0xE8);
ASSERT_OFFSET(NATTraversalEngine, m_ProbeRequests, 0xF4);
ASSERT_OFFSET(NATTraversalEngine, m_RelayPort, 0x118);
ASSERT_SIZE(NATTraversalEngine, 0x120);
} // namespace nex
} // namespace nn
