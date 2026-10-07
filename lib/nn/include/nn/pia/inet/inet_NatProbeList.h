#pragma once

#include "decomp.h"
#include "nn/pia/common/common_ObjList.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/inet/inet_NatProbe.h"

namespace nn {
namespace pia {
namespace transport {
class StationLocation;
} // namespace transport
namespace inet {
// RTTI N2nn3pia4inet12NatProbeListE @ 0x008CF848
// vtable 0x008FFEF0 (vptr 0x008FFEF8), offset_to_top 0, 3 entries
//
// The NAT probes of the stations (at most PROBE_NUM_MAX, the nodes in a buffer of its own). The
// connection id of a probe is the station key of its location. The member names are ours.
class NatProbeList : public ::nn::pia::common::ObjList<nn::pia::inet::NatProbe>
{
public:
    static const u32 PROBE_NUM_MAX = 24;

    NatProbeList(); // 0x003E4E80 | fefates:bytes [tier B]
    virtual ~NatProbeList(); // 0x003E5070 slot 0x00 | fefates:callgraph
    // 0x003E4FB8 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072EFEC slot 0x08

    // a probe from the location arrived; true if it is a new one or its address changed
    bool ProcessProbe(const nn::pia::transport::StationLocation& location, unsigned char kind); // 0x003E4554 | fefates:bytes [tier B]
    void ProcessProbeReply(const nn::pia::transport::StationLocation& location, nn::pia::common::Time sendTime, unsigned char kind); // 0x003E4698 | fefates:bytes [tier B]
    DECOMP_NOINLINE bool AddProbe(const nn::pia::inet::NatProbe& probe); // 0x003E4C14 | fefates:bytes [tier B]

    NatProbe* FindByConnectionId(unsigned int connectionId); // 0x003E4794 | fefates:bytes [tier B]
    NatProbe* FindByAddressAndConnectionId(const nn::pia::transport::StationLocation& location); // 0x003E4900 | fefates:bytes [tier B]
    NatProbe* FindByAddressPortAndConnectionId(const nn::pia::transport::StationLocation& location); // 0x003E4958 | fefates:bytes [tier B]
    // true if one was removed
    bool RemoveProbesByConnectionId(unsigned int connectionId); // 0x003E4868 | fefates:bytes [tier B]
    bool RemoveSameAddressPortAndConnectionIdProbes(const nn::pia::transport::StationLocation& location); // 0x003E49CC | fefates:bytes [tier B]
    DECOMP_NOINLINE void removeNotReceivedProbes(unsigned int connectionId); // 0x003E47C4 | fefates:bytes [tier B]
    DECOMP_NOINLINE void removeDifferentConnectionIdAndSameAddressPortProbes(const nn::pia::transport::StationLocation& location); // 0x003E4AA0 | fefates:bytes [tier B]
    DECOMP_NOINLINE void removeDifferentPortAndSameAddressAndConnectionIdProbes(const nn::pia::transport::StationLocation& location); // 0x003E4B68 | fefates:bytes [tier B]

    // (inline; name is ours) destroys the probe and frees its node
    void RemoveProbe(NatProbe* pProbe)
    {
        pProbe->Trace(0x8000);
        pProbe->~NatProbe();
        Erase(pProbe);
    }

    u8* m_pNodeBuffer; // 0x30
};
ASSERT_OFFSET(NatProbeList, m_pNodeBuffer, 0x30);
ASSERT_SIZE(NatProbeList, 0x34);
} // namespace inet
} // namespace pia
} // namespace nn
