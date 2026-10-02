#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet8NatProbeE @ 0x008CFAA4
// vtable 0x009007F0 (vptr 0x009007F8), offset_to_top 0, 3 entries
class NatProbe : public ::nn::pia::common::RootObject
{
public:
    NatProbe(); // ctor candidate(s) 0x003E45F8, 0x003E4C14, 0x00412E70 (unverified)
    virtual void vf_0x00(); // 0x00412F98 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatProbe
    virtual void vf_0x04(); // 0x00412F78 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatProbe
    virtual void vf_0x08(); // 0x0072FACC slot 0x08 | virtual slot, introduced by nn::pia::inet::NatProbe
    void SprayTargetPort(); // 0x00412C90 | fefates:bytes [tier B]
    void UpdateTargetPort(const nn::pia::transport::StationLocation&); // 0x00412CCC | fefates:bytes [tier B]
    void GetPortSprayCount(); // 0x00412CF0 | fefates:bytes [tier B]
    void UpdateRtt(const nn::pia::common::Time&, const nn::pia::common::Time&); // 0x00412E38 | fefates:bytes [tier B]
    NatProbe(const nn::pia::transport::StationLocation&, const nn::pia::common::Time&, const nn::pia::common::TimeSpan&, unsigned char, unsigned char, unsigned char); // 0x00412E70 | fefates:bytes [tier B]
    void UpdateIsNeeded(nn::pia::common::Time) const; // 0x0072FA74 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
