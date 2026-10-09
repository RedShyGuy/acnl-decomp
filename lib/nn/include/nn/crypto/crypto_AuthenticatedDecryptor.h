#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_CipherMode.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto22AuthenticatedDecryptorE @ 0x008D04D4
//
// Decryption with a MAC. The slot names are the ones of CcmMode, which implements them.
class AuthenticatedDecryptor : public ::nn::crypto::CipherMode
{
public:
    virtual void UpdateAdata(const void* pData, size_t size) = 0;                                 // slot 0x18
    virtual void UpdateAdataFinal() = 0;                                                         // slot 0x1C
    virtual size_t UpdateCdata(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) = 0; // slot 0x20
    virtual size_t UpdateCdataFinal(void* pDst, size_t dstSize) = 0;                             // slot 0x24
    virtual void GenerateMac(void* pMac, size_t macSize) = 0;                                    // slot 0x28

    // all in one: the associated data, the decryption of pSrc to pDst and the comparison of the
    // MAC with pMac; false if pDst is too small or the MAC differs
    bool DecryptAndVerify(void* pDst, size_t dstSize, const void* pAdata, size_t adataSize, const void* pSrc, size_t srcSize, const void* pMac,
                          size_t macSize); // 0x004834A8 | fefates:bytes [tier B]
};
} // namespace crypto
} // namespace nn
