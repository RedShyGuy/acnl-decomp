#pragma once

#include "decomp.h"
#include "nn/pia/inet/inet_NatDetecter.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet15NatPortDetecterE @ 0x008CF890
// vtable 0x00900010 (vptr 0x00900018), offset_to_top 0, 13 entries
class NatPortDetecter : public ::nn::pia::inet::NatDetecter
{
public:
    virtual ~NatPortDetecter(); // 0x003E7E74 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x003E7E48 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatDetecter
    virtual void Startup(nn::pia::common::CallContext*, const nn::pia::common::InetAddress&); // 0x003E3730 slot 0x08 | fefates:bytes
    virtual void Cleanup(); // 0x003E3694 slot 0x0C | slot vf_0x0C of nn::pia::inet::NatDetecter
    virtual void StartSendingMessage(); // 0x003E7C60 slot 0x14 | fefates:bytes
    virtual void CheckAllMessage(); // 0x003E7C50 slot 0x18 | slot vf_0x18 of nn::pia::inet::NatDetecter
    virtual void HandleResult(); // 0x003E7A7C slot 0x1C | slot vf_0x1C of nn::pia::inet::NatDetecter
    virtual void CheckRetry(); // 0x003E79E8 slot 0x20 | fefates:bytes
    virtual void GetDetectionTimeout() const; // 0x0072F0F8 slot 0x24 | slot vf_0x24 of nn::pia::inet::NatDetecter
    virtual void vf_0x30(); // 0x003E7D68 slot 0x30 | fefates:callseq
    NatPortDetecter(); // 0x003E7E10 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
