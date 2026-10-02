#pragma once

#include "decomp.h"
#include "nn/pia/inet/inet_NatDetecter.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet19NatPropertyDetecterE @ 0x008CF930
// vtable 0x00900214 (vptr 0x0090021C), offset_to_top 0, 12 entries
class NatPropertyDetecter : public ::nn::pia::inet::NatDetecter
{
public:
    virtual ~NatPropertyDetecter(); // 0x003F929C slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x003F9248 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatDetecter
    virtual void Startup(nn::pia::common::CallContext*, const nn::pia::common::InetAddress&); // 0x003F9164 slot 0x08 | fefates:callseq
    virtual void Cleanup(); // 0x003F9100 slot 0x0C | fefates:bytes
    virtual void StartSendingMessage(); // 0x003F8F6C slot 0x14 | slot vf_0x14 of nn::pia::inet::NatDetecter
    virtual void CheckAllMessage(); // 0x003F8F40 slot 0x18 | fefates:bytes
    virtual void HandleResult(); // 0x003F8D5C slot 0x1C | slot vf_0x1C of nn::pia::inet::NatDetecter
    virtual void CheckRetry(); // 0x003F8D10 slot 0x20 | fefates:bytes
    virtual void GetDetectionTimeout() const; // 0x0072F168 slot 0x24 | slot vf_0x24 of nn::pia::inet::NatDetecter
    void sendNatPropertyDetectionMessage(); // 0x003F8F84 | fefates:bytes [tier B]
    NatPropertyDetecter(); // 0x003F91FC | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
