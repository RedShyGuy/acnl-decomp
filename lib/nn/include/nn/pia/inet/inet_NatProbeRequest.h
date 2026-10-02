#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet15NatProbeRequestE @ 0x008CF89C
// vtable 0x0090004C (vptr 0x00900054), offset_to_top 0, 3 entries
class NatProbeRequest : public ::nn::pia::common::RootObject
{
public:
    NatProbeRequest(); // ctor candidate(s) 0x003E7E9C, 0x003E7EDC, 0x00405A98, 0x00406558 (unverified)
    virtual void vf_0x00(); // 0x003E7F60 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatProbeRequest
    virtual void vf_0x04(); // 0x003E7F38 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatProbeRequest
    virtual void vf_0x08(); // 0x0072F100 slot 0x08 | virtual slot, introduced by nn::pia::inet::NatProbeRequest
    NatProbeRequest(const nn::pia::transport::StationLocation&); // 0x003E7E9C | fefates:bytes [tier B]
    NatProbeRequest(const nn::pia::transport::StationLocation&, const nn::pia::transport::StationConnectionInfo&, bool, nn::pia::common::CallContext*); // 0x003E7EDC | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
