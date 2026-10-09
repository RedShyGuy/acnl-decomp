#include "nn/crypto/crypto_Api.h"
#include "nn/crypto/crypto_Aes.h"
#include "nn/crypto/crypto_CcmDecryptor.h"
#include "nn/crypto/crypto_CcmEncryptor.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/ssl/detail/detail_Api.h"
#include "nn/ssl/ssl_Api.h"
#include <string.h>

namespace nn {
namespace crypto {
namespace {
// the layout of the encrypted data: nonce (12 bytes, padded to 16), MAC, ciphertext (names ours)
const size_t AES128_KEY_SIZE = 16;
const size_t NONCE_SIZE = 12;
const size_t MAC_OFFSET = 16;
const size_t MAC_SIZE = 16;
const size_t HEADER_SIZE = 32;

inline void PanicIfFailed(nn::Result result)
{
    if (result.IsFailure()) {
        nndbgPanic();
    }
}
} // namespace

// 0x00483674 | fefates:bytes [tier B]
bool DecryptAndVerifyAes128Ccm(void* pDst, const void* pSrc, size_t srcSize, const void* pKey)
{
    const u8* src = static_cast<const u8*>(pSrc);
    size_t dataSize = srcSize - HEADER_SIZE;
    Aes<AES128_KEY_SIZE> aes;
    aes.SetKey(pKey, AES128_KEY_SIZE);
    CcmDecryptor decryptor;
    decryptor.Initialize(aes, src, NONCE_SIZE, 0, dataSize, MAC_SIZE);
    bool isVerified = decryptor.DecryptAndVerify(pDst, dataSize, 0, 0, src + HEADER_SIZE, dataSize, src + MAC_OFFSET, MAC_SIZE);
    decryptor.Finalize();
    return isVerified;
}

// 0x00483724 | fefates:bytes [tier B]
void EncryptAndGenerateAes128Ccm(void* pDst, const void* pSrc, size_t size, const void* pKey)
{
    u8* dst = static_cast<u8*>(pDst);
    Aes<AES128_KEY_SIZE> aes;
    memset(dst, 0, HEADER_SIZE);
    PanicIfFailed(nn::ssl::Initialize());
    PanicIfFailed(nn::ssl::detail::GenerateRandomBytes(dst, NONCE_SIZE));
    PanicIfFailed(nn::ssl::Finalize());
    aes.SetKey(pKey, AES128_KEY_SIZE);
    CcmEncryptor encryptor;
    encryptor.Initialize(aes, dst, NONCE_SIZE, 0, size, MAC_SIZE);
    encryptor.EncryptAndGenerate(dst + MAC_OFFSET, dst + HEADER_SIZE, size, 0, 0, pSrc, size, MAC_SIZE);
    encryptor.Finalize();
}

} // namespace crypto
} // namespace nn
