#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
// the hash of size bytes at pData (defined with the contexts)
void CalculateSha1(void* pOutput, const void* pData, size_t size); // 0x00483278 | fefates:bytes [tier B]
void CalculateSha256(void* pOutput, const void* pData, size_t size); // 0x00140D00 (name is ours, after CalculateSha1)
// AES-128-CCM with a random 12 byte nonce: the destination of the encryption gets the nonce
// (padded to 16 bytes), the 16 byte MAC and the ciphertext; the decryption takes the same layout
bool DecryptAndVerifyAes128Ccm(void* pDst, const void* pSrc, size_t srcSize, const void* pKey); // 0x00483674 | fefates:bytes [tier B]
void EncryptAndGenerateAes128Ccm(void* pDst, const void* pSrc, size_t size, const void* pKey); // 0x00483724 | fefates:bytes [tier B]
} // namespace crypto
} // namespace nn
