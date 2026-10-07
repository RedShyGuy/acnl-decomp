#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet8NatProbeE @ 0x008CFAA4
// vtable 0x009007F0 (vptr 0x009007F8), offset_to_top 0, 3 entries
//
// A NAT traversal attempt to a station location: the port range it sprays, its deadline and the
// round trip time once it answered. The layout is from the constructor; the member names and the
// names marked so are ours.
class NatProbe : public ::nn::pia::common::RootObject
{
public:
    // the round trip time while there is none
    static const s32 RTT_NONE = -1;

    // (inline: in NatProbeList::AddProbe)
    NatProbe() {}
    NatProbe(const nn::pia::transport::StationLocation& location, const nn::pia::common::Time& time, const nn::pia::common::TimeSpan& timeout, unsigned char kind, unsigned char sprayCountMax, unsigned char portRange); // 0x00412E70 | fefates:bytes [tier B]
    virtual ~NatProbe(); // 0x00412F98 slot 0x00
    // 0x00412F78 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0072FACC slot 0x08

    // the next port of the range
    void SprayTargetPort(); // 0x00412C90 | fefates:bytes [tier B]
    void UpdateTargetPort(const nn::pia::transport::StationLocation& location); // 0x00412CCC | fefates:bytes [tier B]
    u16 GetPortSprayCount(); // 0x00412CF0 | fefates:bytes [tier B]
    // false if the location is another station
    bool UpdateTargetAddress(const nn::pia::transport::StationLocation& location); // 0x00412D04
    void UpdateRtt(const nn::pia::common::Time& now, const nn::pia::common::Time& sendTime); // 0x00412E38 | fefates:bytes [tier B]
    bool UpdateIsNeeded(nn::pia::common::Time now) const; // 0x0072FA74 | fefates:bytes [tier B]

    transport::StationLocation m_Location;   // 0x04, its port is the one the next probe goes to
    common::Time m_UpdateTime;               // 0x30
    common::Time m_Deadline;                 // 0x38
    u8 m_Unknown0x40;                        // 0x40
    u8 m_Unknown0x41;                        // 0x41
    u8 m_Unknown0x42;                        // 0x42
    s32 m_Rtt;                               // 0x44, in ms
    u8 m_Kind;                               // 0x48
    u32 m_SprayCount;                        // 0x4C
    u32 m_SprayCountMax;                     // 0x50
    u16 m_PortMin;                           // 0x54
    u16 m_PortMax;                           // 0x56
    common::TimeSpan m_UpdateInterval;       // 0x58 (90 s)
};
ASSERT_OFFSET(NatProbe, m_UpdateTime, 0x30);
ASSERT_OFFSET(NatProbe, m_Rtt, 0x44);
ASSERT_OFFSET(NatProbe, m_PortMin, 0x54);
ASSERT_SIZE(NatProbe, 0x60);

// the probes NatProbeList::ProcessProbe starts (also inet::NexNatTraversalProtocol; the name and the
// member names are ours)
struct NatProbeSetting
{
    u8 m_SprayCountMax; // 0x0 (50)
    s32 m_TimeoutMSec;  // 0x4 (90000)
};
extern NatProbeSetting g_NatProbeSetting;
} // namespace inet
} // namespace pia
} // namespace nn
