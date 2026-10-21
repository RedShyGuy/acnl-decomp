#include "nn/crypto/crypto_CcmDecryptor.h"

namespace nn {
namespace crypto {
// 0x00483234
// 0x00483230 (deleting dtor)
nn::crypto::CcmDecryptor::~CcmDecryptor()
{
    // nothing to do: the members and bases are destroyed by the compiler
}

} // namespace crypto
} // namespace nn
