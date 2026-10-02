#pragma once

#include "decomp.h"
#include "nn/crypto/crypto_CipherMode.h"

namespace nn {
namespace crypto {
// RTTI N2nn6crypto22AuthenticatedEncryptorE @ 0x008D04E0
class AuthenticatedEncryptor : public ::nn::crypto::CipherMode
{
public:
    AuthenticatedEncryptor(); // ctor address unknown
    void EncryptAndGenerate(void*, void*, unsigned int, const void*, unsigned int, const void*, unsigned int, unsigned int); // 0x004835AC | fefates:bytes [tier B]
};
} // namespace crypto
} // namespace nn
