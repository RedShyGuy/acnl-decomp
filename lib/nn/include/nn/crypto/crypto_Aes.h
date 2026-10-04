#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_BlockCipher.h"
#include "nn/crypto/detail/crypto_AesImpl.h"

namespace nn {
namespace crypto {
// Instantiations found in the binary:
//   nn::crypto::Aes<16u>  typeinfo 0x008D04EC  vtable 0x009022F8
//
// AES as a BlockCipher. The functions are inline (the slots of Aes<16> at 0x007E559C, 0x007E5598,
// 0x00828040, 0x00828038, 0x007E55A0, 0x007E5948, 0x007E5668 call the AesImpl functions); the
// member name is ours.
template <size_t KeySize>
class Aes : public BlockCipher
{
public:
    static const size_t BLOCK_SIZE = 16;

    virtual ~Aes() {}
    virtual size_t GetBlockSize() const { return BLOCK_SIZE; }
    virtual size_t GetKeySize() const { return KeySize; }
    virtual void SetKey(const void* pKey, size_t keySize) { m_Impl.SetKey(pKey, keySize); }
    virtual void Encrypt(void* pDst, const void* pSrc) { m_Impl.Encrypt(pDst, pSrc); }
    virtual void Decrypt(void* pDst, const void* pSrc) { m_Impl.Decrypt(pDst, pSrc); }

    detail::AesImpl<KeySize> m_Impl; // 0x04
};
} // namespace crypto
} // namespace nn
