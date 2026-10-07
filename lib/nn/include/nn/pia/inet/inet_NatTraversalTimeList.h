#pragma once

#include "decomp.h"
#include "nn/pia/common/common_FixedObjList.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace inet {
// When the NAT traversal to a location ran (the type name is from the symbols; the layout is from
// NatTraversalTimeList::Add, the member names are ours).
struct NatTraversalTime
{
    // (empty; armlink removed its calls)
    void Trace(u64) const {}

    transport::StationLocation m_Location; // 0x00
    common::Time m_Unknown0x28;            // 0x28
    common::Time m_Unknown0x30;            // 0x30
    u32 m_Unknown0x38;                     // 0x38
    // the time of the port check in milliseconds (NatPortDetecter)
    u16 m_PortCheckMSec;                   // 0x3C
};
ASSERT_SIZE(NatTraversalTime, 0x40);

// RTTI N2nn3pia4inet20NatTraversalTimeListE @ 0x008CF948
// vtable 0x00900350 (vptr 0x00900358), offset_to_top 0, 2 entries
//
// The times of the last NAT traversals, one per location (station key or address).
class NatTraversalTimeList : public ::nn::pia::common::FixedObjList<nn::pia::inet::NatTraversalTime, 12u>
{
public:
    // (inline)
    NatTraversalTimeList() { ClearNodes(); }
    virtual ~NatTraversalTimeList(); // 0x004000D4 slot 0x00
    // 0x00400064 slot 0x04 (deleting dtor)

    // updates the entry of the location or adds one
    void Add(const nn::pia::inet::NatTraversalTime& time); // 0x003FFE2C | fefates:bytes-fuzzy [tier B]
    DECOMP_NOINLINE NatTraversalTime* Find(const nn::pia::transport::StationLocation& location); // 0x003FFF40 | fefates:bytes [tier B]
    void Remove(const nn::pia::transport::StationLocation& location); // 0x003FFFB0 | fefates:bytes [tier B]

    // (inline; name is ours) the same station key or the same address and port
    static bool IsSameLocation(const NatTraversalTime& time, const nn::pia::transport::StationLocation& location)
    {
        return time.m_Location.m_StationKey == location.m_StationKey ||
               time.m_Location.m_StationAddress.m_InetAddress.GetKey() == location.m_StationAddress.m_InetAddress.GetKey();
    }
};
ASSERT_SIZE(NatTraversalTimeList, 0x390);
} // namespace inet
} // namespace pia
} // namespace nn
