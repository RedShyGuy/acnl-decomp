#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/inet/inet_NatDetecter.h"

namespace nn {
namespace nex {
class InetAddress;
} // namespace nex
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet19NatPropertyDetecterE @ 0x008CF930
// vtable 0x00900214 (vptr 0x0090021C), offset_to_top 0, 12 entries
//
// The NAT check at the start of the NAT session: from the replies of the primary server (two
// ports) and of a second server it finds how the NAT maps and filters, and stores that in the nex
// NAT properties. The member names are ours.
class NatPropertyDetecter : public ::nn::pia::inet::NatDetecter
{
public:
    NatPropertyDetecter(); // 0x003F91FC | fefates:bytes [tier B]
    virtual ~NatPropertyDetecter(); // 0x003F929C slot 0x00 | fefates:bytes
    // 0x003F9248 slot 0x04 (deleting dtor)
    virtual nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::common::InetAddress& localAddress); // 0x003F9164 slot 0x08 | fefates:callseq
    virtual void Cleanup(); // 0x003F9100 slot 0x0C | fefates:bytes
    virtual void StartSendingMessage(); // 0x003F8F6C slot 0x14 | slot vf_0x14 of nn::pia::inet::NatDetecter
    virtual bool CheckAllMessage(); // 0x003F8F40 slot 0x18 | fefates:bytes
    virtual bool HandleResult(); // 0x003F8D5C slot 0x1C | slot vf_0x1C of nn::pia::inet::NatDetecter
    virtual bool CheckRetry(); // 0x003F8D10 slot 0x20 | fefates:bytes
    virtual u32 GetDetectionTimeout() const; // 0x0072F168 slot 0x24 | slot vf_0x24 of nn::pia::inet::NatDetecter

    // (armlink placed it behind StartSendingMessage)
    void sendNatPropertyDetectionMessage(); // 0x003F8F84 | fefates:bytes [tier B]

    common::Time m_StartTime;          // 0x3B8
    // for the address strings of the nex NAT properties
    nex::InetAddress* m_pNexAddress;   // 0x3C0
};
ASSERT_OFFSET(NatPropertyDetecter, m_StartTime, 0x3B8);
ASSERT_SIZE(NatPropertyDetecter, 0x3C8);
} // namespace inet
} // namespace pia
} // namespace nn
