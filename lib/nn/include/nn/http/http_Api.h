#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace http {
// connects to http:C and shares size bytes at buffer with it (name is ours)
nn::Result Initialize(uptr buffer, size_t size); // 0x0046F714 (name is ours)
nn::Result Finalize(); // 0x0046FF10 | fefates:bytes [tier B]
} // namespace http
} // namespace nn
