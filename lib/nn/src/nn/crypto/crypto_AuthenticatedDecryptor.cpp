#include "nn/crypto/crypto_AuthenticatedDecryptor.h"
#include <string.h>

namespace nn {
namespace crypto {
// 0x004834A8 | fefates:bytes [tier B]
bool nn::crypto::AuthenticatedDecryptor::DecryptAndVerify(void* pDst, size_t dstSize, const void* pAdata, size_t adataSize, const void* pSrc,
                                                          size_t srcSize, const void* pMac, size_t macSize)
{
    if (adataSize != 0) {
        UpdateAdata(pAdata, adataSize);
        UpdateAdataFinal();
    }
    if (srcSize != 0) {
        size_t size = UpdateCdata(pDst, dstSize, pSrc, srcSize);
        if (size > dstSize) {
            return false;
        }
        if (UpdateCdataFinal(static_cast<u8*>(pDst) + size, dstSize - size) + size > dstSize) {
            return false;
        }
    }
    u8 mac[macSize];
    GenerateMac(mac, macSize);
    return memcmp(mac, pMac, macSize) == 0;
}

} // namespace crypto
} // namespace nn
