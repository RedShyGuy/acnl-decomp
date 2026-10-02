#include "nn/crypto/crypto_HashContextBase.h"
#include "nn/crypto/crypto_ShaBlock512BitContext.h"

namespace nn {
namespace crypto {
// ctor address unknown
nn::crypto::ShaBlock512BitContext::ShaBlock512BitContext()
{
}

// 0x0014375C | nintendogs:bytes [tier A]
void nn::crypto::ShaBlock512BitContext::AddPadding()
{
}

// 0x001437E4 | nintendogs:bytes [tier A]
void nn::crypto::ShaBlock512BitContext::Update(const void*, unsigned)
{
}

} // namespace crypto
} // namespace nn
