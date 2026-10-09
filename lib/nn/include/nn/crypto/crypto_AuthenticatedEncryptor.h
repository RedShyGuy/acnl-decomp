#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_CipherMode.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto22AuthenticatedEncryptorE @ 0x008D04E0
//
// Encryption with a MAC. The slot names are the ones of CcmMode, which implements them.
class AuthenticatedEncryptor : public ::nn::crypto::CipherMode
{
public:
    virtual void UpdateAdata(const void* pData, size_t size) = 0;                                 // slot 0x18
    virtual void UpdateAdataFinal() = 0;                                                         // slot 0x1C
    virtual size_t UpdatePdata(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) = 0; // slot 0x20
    virtual size_t UpdatePdataFinal(void* pDst, size_t dstSize) = 0;                             // slot 0x24
    virtual void GenerateMac(void* pMac, size_t macSize) = 0;                                    // slot 0x28

    // all in one: the associated data, the encryption of pSrc to pDst and the MAC (nothing more
    // if pDst is too small)
    void EncryptAndGenerate(void* pMac, void* pDst, size_t dstSize, const void* pAdata, size_t adataSize, const void* pSrc, size_t srcSize,
                            size_t macSize); // 0x004835AC | fefates:bytes [tier B]
};
} // namespace crypto
} // namespace nn
