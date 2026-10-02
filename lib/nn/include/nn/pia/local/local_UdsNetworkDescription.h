#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalNetworkDescription.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21UdsNetworkDescriptionE @ 0x008CFC28
// vtable 0x00900F58 (vptr 0x00900F60), offset_to_top 0, 8 entries
class UdsNetworkDescription : public ::nn::pia::local::LocalNetworkDescription
{
public:
    UdsNetworkDescription(); // ctor address unknown
    virtual void GetCurrentParticipants() const; // 0x00731530 slot 0x00 | fefates:bytes
    virtual void GetMaxParticipants() const; // 0x0073151C slot 0x04 | fefates:bytes
    virtual void IsOpened() const; // 0x00731598 slot 0x08 | fefates:bytes
    virtual void GetLocalCommunicationId() const; // 0x00731544 slot 0x0C | fefates:bytes
    virtual void GetSubId() const; // 0x00731584 slot 0x10 | fefates:bytes
    virtual void vf_0x14(); // 0x00731508 slot 0x14 | virtual slot, introduced by nn::pia::local::UdsNetworkDescription
    virtual void vf_0x18(); // 0x0073155C slot 0x18 | virtual slot, introduced by nn::pia::local::UdsNetworkDescription
    virtual void vf_0x1C(); // 0x0041D5B4 slot 0x1C | virtual slot, introduced by nn::pia::local::UdsNetworkDescription
};
} // namespace local
} // namespace pia
} // namespace nn
