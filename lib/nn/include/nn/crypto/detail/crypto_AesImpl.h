#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
namespace detail {
// AES with a key of KeySize bytes: the expanded key. The class and its functions are from the
// symbols; the member is ours.
template <int KeySize>
class AesImpl
{
public:
    // AesImpl<16>: 0x007E55A8
    void SetKey(const void* pKey, size_t keySize);
    // AesImpl<16>: 0x007E5950 | 0x007E5670
    void Encrypt(void* pDst, const void* pSrc);
    void Decrypt(void* pDst, const void* pSrc);

    u32 m_RoundKey[(KeySize / 4 + 7) * 4]; // the round keys
};
} // namespace detail
} // namespace crypto
} // namespace nn
