#include "nn/crypto/crypto_CipherMode.h"
#include "nn/crypto/crypto_AuthenticatedDecryptor.h"

namespace nn {
namespace crypto {
// ctor address unknown
nn::crypto::AuthenticatedDecryptor::AuthenticatedDecryptor()
{
}

// 0x004834A8 | fefates:bytes [tier B]
void nn::crypto::AuthenticatedDecryptor::DecryptAndVerify(void*, unsigned int, const void*, unsigned int, const void*, unsigned int, const void*, unsigned int)
{
}

} // namespace crypto
} // namespace nn
