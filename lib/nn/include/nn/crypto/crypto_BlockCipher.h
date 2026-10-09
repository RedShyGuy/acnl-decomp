#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto11BlockCipherE @ 0x008D0488
//
// Interface of a block cipher. The slots are the ones of Aes<16> (pia's Crypto calls slot 0x0C
// and 0x10); their names are ours. Encrypt and Decrypt are const: CtrMode and CcmMode get the
// cipher as a const reference / pointer and call Encrypt.
class BlockCipher
{
public:
    virtual ~BlockCipher() {}
    virtual size_t GetBlockSize() const = 0;                      // slot 0x08
    virtual size_t GetKeySize() const = 0;                        // slot 0x0C
    virtual void SetKey(const void* pKey, size_t keySize) = 0;    // slot 0x10
    virtual void Encrypt(void* pDst, const void* pSrc) const = 0; // slot 0x14
    virtual void Decrypt(void* pDst, const void* pSrc) const = 0; // slot 0x18
};
} // namespace crypto
} // namespace nn
