#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
void CalculateSha1(void*, const void*, unsigned int); // 0x00483278 | fefates:bytes [tier B]
void DecryptAndVerifyAes128Ccm(void*, const void*, unsigned int, const void*); // 0x00483674 | fefates:bytes [tier B]
void EncryptAndGenerateAes128Ccm(void*, const void*, unsigned int, const void*); // 0x00483724 | fefates:bytes [tier B]
} // namespace crypto
} // namespace nn
