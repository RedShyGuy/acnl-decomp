#include "nn/crypto/detail/crypto_CcmMode.h"

namespace nn {
namespace crypto {
namespace detail {
// 0x0048389C | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::Initialize(const nn::crypto::BlockCipher&, const void*, unsigned int, unsigned int, unsigned int, unsigned int)
{
}

// 0x004839E0 | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::GenerateMac(void*, unsigned int)
{
}

// 0x00483A4C | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::UpdateAdata(const void*, unsigned int)
{
}

// 0x00483B0C | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::UpdateCdata(void*, unsigned int, const void*, unsigned int)
{
}

// 0x00483C5C | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::UpdatePdata(void*, unsigned int, const void*, unsigned int)
{
}

// 0x00483DB0 | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::UpdateAdataFinal()
{
}

// 0x00483E00 | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::UpdateCdataFinal(void*, unsigned int)
{
}

// 0x00483EA0 | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::UpdatePdataFinal(void*, unsigned int)
{
}

// 0x00483F44 | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::Finalize()
{
}

} // namespace detail
} // namespace crypto
} // namespace nn
