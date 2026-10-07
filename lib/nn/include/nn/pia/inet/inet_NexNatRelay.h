#pragma once

#include "decomp.h"
#include "nn/pia/inet/inet_NatRelayInterface.h"
#include "nn/pia/inet/inet_NexNatRelayInterface.h"

namespace nn {
namespace nex {
class InetAddress;
class ProtocolCallContext;
class StationURL;
template <typename T> class qList;
} // namespace nex
namespace pia {
namespace inet {
class NexNatTraversalProtocol;

// RTTI N2nn3pia4inet11NexNatRelayE @ 0x008CF830
// vtable 0x008FFEB8 (vptr 0x008FFEC0), offset_to_top 0, 8 entries
//
// The NAT relay of pia over the relay client of nex (NexFacade::m_pNatRelay): probe requests and
// reports go to the nex server, the requests of the server come back through m_RelayInterface.
// The layout is from the constructor; the member names are ours.
class NexNatRelay : public ::nn::pia::inet::NatRelayInterface
{
public:
    NexNatRelay(); // 0x003E3F28 | fefates:bytes [tier B]
    virtual ~NexNatRelay(); // 0x003E4154 slot 0x00 | fefates:bytes
    // 0x003E4144 slot 0x04 (deleting dtor)
    virtual void RequestProbe(const nn::pia::transport::StationLocation& location); // 0x003E3B50 slot 0x08 | slot vf_0x08 of nn::pia::inet::NexNatRelay
    virtual void RelayProbeRequest(const nn::pia::transport::StationLocation& target, const nn::pia::transport::StationLocation& self); // 0x003E3B6C slot 0x0C | slot vf_0x0C of nn::pia::inet::NexNatRelay
    virtual void ReportNatTraversalResult(const unsigned int& stationKey, const bool& isSucceeded, const unsigned int& rtt); // 0x003E3EC4 slot 0x10 | fefates:bytes
    virtual void ReportNatProperties(const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt); // 0x003E3E48 slot 0x14 | fefates:bytes
    virtual void SetLocalCID(unsigned int cid); // 0x003E3B3C slot 0x18 | slot vf_0x18 of nn::pia::inet::NexNatRelay
    virtual void AssociateProtocol(nn::pia::inet::NexNatTraversalProtocol* pProtocol); // 0x003E3B64 slot 0x1C | slot vf_0x1C of nn::pia::inet::NexNatRelay

    // the targets of a probe request (one URL) and the source
    nex::qList<nex::StationURL>* m_pTargetUrls;  // 0x04
    nex::StationURL* m_pSourceUrl;               // 0x08
    nex::InetAddress* m_pNexAddress;             // 0x0C
    nex::ProtocolCallContext* m_pCallContext;    // 0x10
    NexNatTraversalProtocol* m_pProtocol;        // 0x14
    NexNatRelayInterface m_RelayInterface;       // 0x18
};
ASSERT_OFFSET(NexNatRelay, m_RelayInterface, 0x18);
ASSERT_SIZE(NexNatRelay, 0x28);
} // namespace inet
} // namespace pia
} // namespace nn
