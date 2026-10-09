#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_AuthenticatedDecryptor.h"
#include "nn/crypto/detail/crypto_CcmMode.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto12CcmDecryptorE @ 0x008D049C
// vtable 0x00902264 (vptr 0x0090226C), offset_to_top 0, 11 entries
//
// The slots forward to the CcmMode (the member name is ours).
class CcmDecryptor : public ::nn::crypto::AuthenticatedDecryptor
{
public:
    // (inline: DecryptAndVerifyAes128Ccm calls CcmMode::Initialize; name is ours)
    void Initialize(const BlockCipher& cipher, const void* pNonce, size_t nonceSize, size_t adataSize, size_t pdataSize, size_t macSize)
    {
        m_Mode.Initialize(cipher, pNonce, nonceSize, adataSize, pdataSize, macSize);
    }

    virtual ~CcmDecryptor();
    virtual void Finalize() { m_Mode.Finalize(); } // 0x00483F3C
    virtual size_t vf_0x0C() const { return 13; } // 0x00737580
    virtual size_t vf_0x10() const { return 1; } // 0x00737578
    virtual size_t vf_0x14() const { return 16; } // 0x00737570
    virtual void UpdateAdata(const void* pData, size_t size) { m_Mode.UpdateAdata(pData, size); } // 0x00483A44
    virtual void UpdateAdataFinal() { m_Mode.UpdateAdataFinal(); } // 0x00483DA8
    virtual size_t UpdateCdata(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) { return m_Mode.UpdateCdata(pDst, dstSize, pSrc, srcSize); } // 0x00483218
    virtual size_t UpdateCdataFinal(void* pDst, size_t dstSize) { return m_Mode.UpdateCdataFinal(pDst, dstSize); } // 0x00483DF8
    virtual void GenerateMac(void* pMac, size_t macSize) { m_Mode.GenerateMac(pMac, macSize); } // 0x004839D8

    detail::CcmMode m_Mode; // 0x04
};
ASSERT_SIZE(CcmDecryptor, 0x3C);
} // namespace crypto
} // namespace nn
