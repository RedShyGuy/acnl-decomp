#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
namespace detail {
// AES (FIPS 197) with a key of KeySize bytes, with the round tables. The class and its functions
// are from the symbols; the members are ours. The round keys are kept either for encryption or
// for decryption (round keys 1 to ROUND_COUNT - 1 with InvMixColumns applied); Encrypt and Decrypt
// convert them when needed.
template <int KeySize>
class AesImpl
{
public:
    static const int KEY_WORD_COUNT = KeySize / 4;
    static const int ROUND_COUNT = KEY_WORD_COUNT + 6;

    // (only a key of KeySize bytes is taken)
    void SetKey(const void* pKey, size_t keySize);
    void Encrypt(void* pDst, const void* pSrc);
    void Decrypt(void* pDst, const void* pSrc);

    u32 m_RoundKey[(ROUND_COUNT + 1) * 4]; // 0x00
    bool m_IsEncryptionKey;                // 0xB0 for AesImpl<16>
};
} // namespace detail
} // namespace crypto
} // namespace nn
