#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local29UdsAroundNetworkSearchManagerE @ 0x008CFD48
// vtable 0x009012F0 (vptr 0x009012F8), offset_to_top 0, 10 entries
class UdsAroundNetworkSearchManager : public ::nn::pia::local::LocalAroundNetworkSearchManager
{
public:
    UdsAroundNetworkSearchManager(); // ctor address unknown
    virtual ~UdsAroundNetworkSearchManager(); // 0x004249BC slot 0x00 | slot vf_0x00 of nn::pia::local::LocalAroundNetworkSearchManager
    virtual void vf_0x04(); // 0x00423604 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalAroundNetworkSearchManager
    virtual void CreateLocalAroundNetworkInfo(); // 0x0042343C slot 0x10 | fefates:bytes
    virtual void CreateLocalAroundNetworkSearchBackgroundJob(); // 0x00423588 slot 0x14 | fefates:bytes
    virtual void vf_0x18(); // 0x007316E4 slot 0x18 | virtual slot, introduced by nn::pia::local::LocalAroundNetworkSearchManager
    virtual void SerializeAroundNetworkStatus(nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage*, const nn::pia::local::LocalAroundNetworkSearchManager::AroundNetworkStatus*) const; // 0x004149FC slot 0x1C | fefates:bytes
    virtual void vf_0x20(); // 0x00423484 slot 0x20 | fefates:callseq
    virtual void vf_0x24(); // 0x00731768 slot 0x24 | virtual slot, introduced by nn::pia::local::LocalAroundNetworkSearchManager
};
} // namespace local
} // namespace pia
} // namespace nn
