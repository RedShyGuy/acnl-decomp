#pragma once

#include "decomp.h"
#include "nn/pia/common/common_ObjList.h"
#include "nn/pia/inet/inet_NatProbe.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet12NatProbeListE @ 0x008CF848
// vtable 0x008FFEF0 (vptr 0x008FFEF8), offset_to_top 0, 3 entries
class NatProbeList : public ::nn::pia::common::ObjList<nn::pia::inet::NatProbe>
{
public:
    virtual void vf_0x00(); // 0x003E5070 slot 0x00 | fefates:callgraph
    virtual void vf_0x04(); // 0x003E4FB8 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatProbeList
    virtual void vf_0x08(); // 0x0072EFEC slot 0x08 | virtual slot, introduced by nn::pia::inet::NatProbeList
    void ProcessProbe(const nn::pia::transport::StationLocation&, unsigned char); // 0x003E4554 | fefates:bytes [tier B]
    void ProcessProbeReply(const nn::pia::transport::StationLocation&, nn::pia::common::Time, unsigned char); // 0x003E4698 | fefates:bytes [tier B]
    void FindByConnectionId(unsigned int); // 0x003E4794 | fefates:bytes [tier B]
    void removeNotReceivedProbes(unsigned int); // 0x003E47C4 | fefates:bytes [tier B]
    void RemoveProbesByConnectionId(unsigned int); // 0x003E4868 | fefates:bytes [tier B]
    void FindByAddressAndConnectionId(const nn::pia::transport::StationLocation&); // 0x003E4900 | fefates:bytes [tier B]
    void FindByAddressPortAndConnectionId(const nn::pia::transport::StationLocation&); // 0x003E4958 | fefates:bytes [tier B]
    void RemoveSameAddressPortAndConnectionIdProbes(const nn::pia::transport::StationLocation&); // 0x003E49CC | fefates:bytes [tier B]
    void removeDifferentConnectionIdAndSameAddressPortProbes(const nn::pia::transport::StationLocation&); // 0x003E4AA0 | fefates:bytes [tier B]
    void removeDifferentPortAndSameAddressAndConnectionIdProbes(const nn::pia::transport::StationLocation&); // 0x003E4B68 | fefates:bytes [tier B]
    void AddProbe(const nn::pia::inet::NatProbe&); // 0x003E4C14 | fefates:bytes [tier B]
    NatProbeList(); // 0x003E4E80 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
