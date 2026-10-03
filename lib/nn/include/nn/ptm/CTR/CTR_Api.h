#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace ptm {
namespace CTR {
// connect to / disconnect from ptm:u (nothing happens if already done)
nn::Result Initialize(); // 0x0011E2F8 | nintendogs:bytes [tier B]
nn::Result Finalize(); // 0x0013112C | nintendogs:bytes [tier B]
} // namespace CTR
} // namespace ptm
} // namespace nn
