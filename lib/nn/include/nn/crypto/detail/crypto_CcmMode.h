#pragma once

#include "decomp.h"

namespace nn {
namespace crypto {
class BlockCipher;

namespace detail {
// CCM (NIST SP 800-38C) over a block cipher with 16 byte blocks: the CBC-MAC and the counter
// mode in one pass. Pdata is the plaintext, Cdata the ciphertext, Adata the associated data
// (names from the symbols). The members and their names are ours.
class CcmMode
{
public:
    static const size_t BLOCK_SIZE = 16;

    // the sizes of all data are given here (they are part of the first MAC block)
    void Initialize(const nn::crypto::BlockCipher& cipher, const void* pNonce, size_t nonceSize, size_t adataSize, size_t pdataSize,
                    size_t macSize); // 0x0048389C | fefates:bytes [tier B]
    // the first macSize bytes (at most m_MacSize) of the MAC
    void GenerateMac(void* pMac, size_t macSize); // 0x004839E0 | fefates:bytes [tier B]
    void UpdateAdata(const void* pData, size_t size); // 0x00483A4C | fefates:bytes [tier B]
    // The Update*data functions return the bytes written to pDst; if pDst is too small, nothing
    // is written and the needed size is returned.
    size_t UpdateCdata(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize); // 0x00483B0C | fefates:bytes [tier B]
    size_t UpdatePdata(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize); // 0x00483C5C | fefates:bytes [tier B]
    void UpdateAdataFinal(); // 0x00483DB0 | fefates:bytes [tier B]
    size_t UpdateCdataFinal(void* pDst, size_t dstSize); // 0x00483E00 | fefates:bytes [tier B]
    size_t UpdatePdataFinal(void* pDst, size_t dstSize); // 0x00483EA0 | fefates:bytes [tier B]
    // clears everything
    void Finalize(); // 0x00483F44 | fefates:bytes [tier B]

    const nn::crypto::BlockCipher* m_pCipher; // 0x00
    s8 m_BufferUsed;                          // 0x04, bytes in m_Buffer
    s8 m_NonceSize;                           // 0x05
    s8 m_MacSize;                             // 0x06
    DECOMP_ALIGN(4) u8 m_Buffer[BLOCK_SIZE];  // 0x08, the data of an incomplete block
    u8 m_Mac[BLOCK_SIZE];                     // 0x18, the CBC-MAC so far
    u8 m_Counter[BLOCK_SIZE];                 // 0x28
};
ASSERT_OFFSET(CcmMode, m_Buffer, 0x08);
ASSERT_SIZE(CcmMode, 0x38);
} // namespace detail
} // namespace crypto
} // namespace nn
