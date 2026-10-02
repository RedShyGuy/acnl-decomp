#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace nwm {
namespace CTR {
nn::Result GetMacAddress(nn::nwm::Mac& mac); // 0x003E23D4 | fefates:bytes [tier B]
} // namespace CTR
} // namespace nwm
} // namespace nn
