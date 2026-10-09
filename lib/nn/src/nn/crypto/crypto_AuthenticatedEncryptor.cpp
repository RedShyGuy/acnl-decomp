#include "nn/crypto/crypto_AuthenticatedEncryptor.h"

namespace nn {
namespace crypto {
// 0x004835AC | fefates:bytes [tier B]
void nn::crypto::AuthenticatedEncryptor::EncryptAndGenerate(void* pMac, void* pDst, size_t dstSize, const void* pAdata, size_t adataSize,
                                                            const void* pSrc, size_t srcSize, size_t macSize)
{
    if (adataSize != 0) {
        UpdateAdata(pAdata, adataSize);
        UpdateAdataFinal();
    }
    if (srcSize != 0) {
        size_t size = UpdatePdata(pDst, dstSize, pSrc, srcSize);
        if (size > dstSize) {
            return;
        }
        if (UpdatePdataFinal(static_cast<u8*>(pDst) + size, dstSize - size) + size > dstSize) {
            return;
        }
    }
    GenerateMac(pMac, macSize);
}

} // namespace crypto
} // namespace nn
