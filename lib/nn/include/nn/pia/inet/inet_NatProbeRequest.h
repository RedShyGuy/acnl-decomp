#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace common {
class CallContext;
} // namespace common
namespace inet {
// RTTI N2nn3pia4inet15NatProbeRequestE @ 0x008CF89C
// vtable 0x0090004C (vptr 0x00900054), offset_to_top 0, 3 entries
//
// A NAT traversal to start (NexNatTraversalProtocol): the location, the connection info of the
// station and the call context of the caller. The layout is from the constructors; the member
// names are ours.
class NatProbeRequest : public ::nn::pia::common::RootObject
{
public:
    // (inline: in NatProbeRequestList)
    NatProbeRequest() {}
    NatProbeRequest(const nn::pia::transport::StationLocation& location); // 0x003E7E9C | fefates:bytes [tier B]
    NatProbeRequest(const nn::pia::transport::StationLocation& location, const nn::pia::transport::StationConnectionInfo& info, bool isInverse, nn::pia::common::CallContext* pCallContext); // 0x003E7EDC | fefates:bytes [tier B]
    virtual ~NatProbeRequest(); // 0x003E7F60 slot 0x00
    // 0x003E7F38 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072F100 slot 0x08

    transport::StationLocation m_Location;            // 0x04
    transport::StationConnectionInfo m_ConnectionInfo; // 0x2C
    bool m_IsInverse;                                 // 0x80
    common::Time m_Unknown0x88;                       // 0x88
    common::CallContext* m_pCallContext;              // 0x90
};
ASSERT_OFFSET(NatProbeRequest, m_IsInverse, 0x80);
ASSERT_OFFSET(NatProbeRequest, m_pCallContext, 0x90);
ASSERT_SIZE(NatProbeRequest, 0x98);
} // namespace inet
} // namespace pia
} // namespace nn
