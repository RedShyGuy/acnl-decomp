#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
namespace detail {
namespace anonymous_namespace {
void ProcessCbcBlocks(void*, const void*, int, const nn::crypto::BlockCipher*); // 0x0048380C | fefates:bytes [tier B]
} // namespace anonymous_namespace
} // namespace detail
} // namespace crypto
} // namespace nn
