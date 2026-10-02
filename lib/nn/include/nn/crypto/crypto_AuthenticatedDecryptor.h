#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_CipherMode.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto22AuthenticatedDecryptorE @ 0x008D04D4
class AuthenticatedDecryptor : public ::nn::crypto::CipherMode
{
public:
    AuthenticatedDecryptor(); // ctor address unknown
    void DecryptAndVerify(void*, unsigned int, const void*, unsigned int, const void*, unsigned int, const void*, unsigned int); // 0x004834A8 | fefates:bytes [tier B]
};
} // namespace crypto
} // namespace nn
