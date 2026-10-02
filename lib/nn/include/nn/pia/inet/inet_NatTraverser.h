#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet12NatTraverserE @ 0x008CF860
// vtable 0x008FFF04 (vptr 0x008FFF0C), offset_to_top 0, 5 entries
class NatTraverser : public ::nn::pia::common::RootObject
{
public:
    virtual void vf_0x00(); // 0x003E69A8 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatTraverser
    virtual ~NatTraverser(); // 0x003E68B4 slot 0x04 | fefates:bytes
    virtual void Startup(nn::pia::common::CallContext*); // 0x003E6714 slot 0x08 | slot vf_0x08 of nn::pia::inet::NatTraverser
    virtual void Cleanup(); // 0x003E6694 slot 0x0C | fefates:bytes
    virtual void vf_0x10(); // 0x0072EFF0 slot 0x10 | virtual slot, introduced by nn::pia::inet::NatTraverser
    void CreateProtocols(); // 0x003E526C | fefates:bytes [tier B]
    void StartNatTraversal(); // 0x003E53F8 | fefates:bytes [tier B]
    void IsStartupCancelled(); // 0x003E5438 | fefates:bytes [tier B]
    void UpdateNatServerAddress(); // 0x003E56D4 | fefates:bytes [tier B]
    void CheckLatestStationLocation(nn::pia::transport::StationLocation&); // 0x003E651C | fefates:bytes [tier B]
    NatTraverser(); // 0x003E67D8 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
