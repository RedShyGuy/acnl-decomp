#include "nn/crypto/crypto_CcmEncryptor.h"

namespace nn {
namespace crypto {
// 0x00483274
// 0x00483270 (deleting dtor)
nn::crypto::CcmEncryptor::~CcmEncryptor()
{
    // nothing to do: the members and bases are destroyed by the compiler
}

} // namespace crypto
} // namespace nn
