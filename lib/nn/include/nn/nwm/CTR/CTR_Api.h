#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace nwm {
namespace CTR {
nn::Result GetMacAddress(nn::nwm::Mac& mac); // 0x003E23D4 | fefates:bytes [tier B]
// the wifi link level from the shared page (0x1FF81066; name is ours, the field after 3dbrew)
u8 GetWifiLinkLevel(); // 0x003E23C0
// the wifi state from the shared page (0x1FF81067; name is ours)
u8 GetWifiState(); // 0x003E242C (name is ours)
} // namespace CTR
} // namespace nwm
} // namespace nn
