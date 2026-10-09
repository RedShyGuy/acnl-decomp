#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_AuthenticatedEncryptor.h"
#include "nn/crypto/detail/crypto_CcmMode.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto12CcmEncryptorE @ 0x008D04A8
// vtable 0x00902298 (vptr 0x009022A0), offset_to_top 0, 11 entries
//
// The slots forward to the CcmMode (the member name is ours).
class CcmEncryptor : public ::nn::crypto::AuthenticatedEncryptor
{
public:
    // (inline: EncryptAndGenerateAes128Ccm calls CcmMode::Initialize; name is ours)
    void Initialize(const BlockCipher& cipher, const void* pNonce, size_t nonceSize, size_t adataSize, size_t pdataSize, size_t macSize)
    {
        m_Mode.Initialize(cipher, pNonce, nonceSize, adataSize, pdataSize, macSize);
    }

    virtual ~CcmEncryptor();
    virtual void Finalize() { m_Mode.Finalize(); } // 0x00483268
    virtual size_t vf_0x0C() const { return 13; } // 0x00737598
    virtual size_t vf_0x10() const { return 1; } // 0x00737590
    virtual size_t vf_0x14() const { return 16; } // 0x00737588
    virtual void UpdateAdata(const void* pData, size_t size) { m_Mode.UpdateAdata(pData, size); } // 0x00483240
    virtual void UpdateAdataFinal() { m_Mode.UpdateAdataFinal(); } // 0x00483260
    virtual size_t UpdatePdata(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize) { return m_Mode.UpdatePdata(pDst, dstSize, pSrc, srcSize); } // 0x00483248
    virtual size_t UpdatePdataFinal(void* pDst, size_t dstSize) { return m_Mode.UpdatePdataFinal(pDst, dstSize); } // 0x00483E98
    virtual void GenerateMac(void* pMac, size_t macSize) { m_Mode.GenerateMac(pMac, macSize); } // 0x00483238

    detail::CcmMode m_Mode; // 0x04
};
ASSERT_SIZE(CcmEncryptor, 0x3C);
} // namespace crypto
} // namespace nn
