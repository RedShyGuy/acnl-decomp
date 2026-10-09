#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
class BlockCipher;

namespace detail {
// The counter mode with blocks of BlockSize bytes (class and functions from the symbols).
template <int BlockSize>
class CtrMode
{
public:
    // dst = src ^ E(counter) for count blocks; the counter is incremented after each block
    static void ProcessBlocks(void* pDst, void* pCounter, const void* pSrc, int count, const nn::crypto::BlockCipher& cipher);
    // increments the big endian counter (byte 0 is not part of it)
    static void IncrementCounter(void* pCounter);
};
} // namespace detail
} // namespace crypto
} // namespace nn
