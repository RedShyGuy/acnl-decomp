#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/inet/inet_NatDetecter.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace inet {
class NexNatTraversalProtocol;

// RTTI N2nn3pia4inet15NatPortDetecterE @ 0x008CF890
// vtable 0x00900010 (vptr 0x00900018), offset_to_top 0, 13 entries
//
// The port check: the port the NAT maps the socket to (as the primary server sees it) goes into
// the nex NAT properties; for an inverse NAT traversal the predicted address goes into the own
// location and a probe request with it goes to the target through the relay. The member names
// are ours.
class NatPortDetecter : public ::nn::pia::inet::NatDetecter
{
public:
    // the last perceived port; a smaller one means the NAT wrapped around
    static u16 s_LastPerceivedPort; // 0x0097FA08

    NatPortDetecter(); // 0x003E7E10 | fefates:bytes [tier B]
    virtual ~NatPortDetecter(); // 0x003E7E74 slot 0x00 | fefates:bytes
    // 0x003E7E48 slot 0x04 (deleting dtor)
    virtual nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::common::InetAddress& localAddress); // 0x003E3730 slot 0x08 | fefates:bytes
    // (armlink placed it in front of NatDetecter::Cleanup)
    virtual void Cleanup(); // 0x003E3694 slot 0x0C | slot vf_0x0C of nn::pia::inet::NatDetecter
    virtual void StartSendingMessage(); // 0x003E7C60 slot 0x14 | fefates:bytes
    virtual bool CheckAllMessage(); // 0x003E7C50 slot 0x18 | slot vf_0x18 of nn::pia::inet::NatDetecter
    virtual bool HandleResult(); // 0x003E7A7C slot 0x1C | slot vf_0x1C of nn::pia::inet::NatDetecter
    virtual bool CheckRetry(); // 0x003E79E8 slot 0x20 | fefates:bytes
    virtual u32 GetDetectionTimeout() const; // 0x0072F0F8 slot 0x24 | slot vf_0x24 of nn::pia::inet::NatDetecter
    virtual nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::common::InetAddress& localAddress, const nn::pia::transport::StationLocation& target, const nn::pia::transport::StationLocation& self, nn::pia::inet::NexNatTraversalProtocol* pProtocol, bool isInverse); // 0x003E7D68 slot 0x30 | fefates:callseq

    transport::StationLocation m_TargetLocation; // 0x3B8
    // the predicted address goes into its station address
    transport::StationLocation m_SelfLocation;   // 0x3E0
    NexNatTraversalProtocol* m_pProtocol;        // 0x408
    bool m_IsInverse;                            // 0x40C
    common::Time m_StartTime;                    // 0x410
};
ASSERT_OFFSET(NatPortDetecter, m_TargetLocation, 0x3B8);
ASSERT_OFFSET(NatPortDetecter, m_StartTime, 0x410);
ASSERT_SIZE(NatPortDetecter, 0x418);
} // namespace inet
} // namespace pia
} // namespace nn
