#include "nn/crypto/detail/crypto_CtrMode.h"
#include "nn/crypto/crypto_BlockCipher.h"

namespace nn {
namespace crypto {
namespace detail {
template <int BlockSize>
void CtrMode<BlockSize>::ProcessBlocks(void* pDst, void* pCounter, const void* pSrc, int count, const nn::crypto::BlockCipher& cipher)
{
    u32* dst = static_cast<u32*>(pDst);
    const u32* src = static_cast<const u32*>(pSrc);
    for (int i = 0; i < count; i++) {
        u32 key[BlockSize / sizeof(u32)];
        cipher.Encrypt(key, pCounter);
        for (size_t j = 0; j < BlockSize / sizeof(u32); j++) {
            dst[j] = src[j] ^ key[j];
        }
        IncrementCounter(pCounter);
        src += BlockSize / sizeof(u32);
        dst += BlockSize / sizeof(u32);
    }
}

template <int BlockSize>
void CtrMode<BlockSize>::IncrementCounter(void* pCounter)
{
    u8* counter = static_cast<u8*>(pCounter);
    for (int i = BlockSize - 1; i > 0; i--) {
        if (++counter[i] != 0) {
            break;
        }
    }
}

// 0x007E5C7C | fefates:bytes [tier B]
template void CtrMode<16>::ProcessBlocks(void* pDst, void* pCounter, const void* pSrc, int count, const nn::crypto::BlockCipher& cipher);
// 0x007E5D3C | fefates:bytes [tier B]
template void CtrMode<16>::IncrementCounter(void* pCounter);

} // namespace detail
} // namespace crypto
} // namespace nn
