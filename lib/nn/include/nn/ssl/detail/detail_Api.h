#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace ssl {
namespace detail {
// random bytes from ssl:C; RESULT_NOT_INITIALIZED before nn::ssl::Initialize
nn::Result GenerateRandomBytes(u8* buffer, size_t size); // 0x004673B8 | fefates:bytes [tier B]
} // namespace detail
} // namespace ssl
} // namespace nn
