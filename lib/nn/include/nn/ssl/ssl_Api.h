#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace ssl {
// counted: the first call connects to ssl:C, the last Finalize closes the session
nn::Result Initialize(); // 0x00467204 | fefates:bytes [tier B]
nn::Result Finalize(); // 0x004673E8 | fefates:bytes [tier B]
} // namespace ssl
} // namespace nn
