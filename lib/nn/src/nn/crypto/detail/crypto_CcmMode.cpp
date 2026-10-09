#include "nn/crypto/detail/crypto_CcmMode.h"
#include "nn/crypto/crypto_BlockCipher.h"
#include "nn/crypto/detail/crypto_CtrMode.h"
#include <string.h>

namespace nn {
namespace crypto {
namespace detail {
namespace {
// the flags of the first MAC block
const u8 FLAG_ADATA = 0x40;

// the size of the associated data before it in the MAC: 2 bytes below 0xFF00, else 0xFF 0xFE and 4 bytes
const size_t ADATA_SIZE_LIMIT = 0xFF00;

// mac = E(mac ^ block) for count blocks
// 0x0048380C | fefates:bytes [tier B]
DECOMP_NOINLINE void ProcessCbcBlocks(void* pMac, const void* pSrc, int count, const nn::crypto::BlockCipher* pCipher)
{
    u32* mac = static_cast<u32*>(pMac);
    const u32* src = static_cast<const u32*>(pSrc);
    for (int i = 0; i < count; i++) {
        u32 block[CcmMode::BLOCK_SIZE / sizeof(u32)];
        block[0] = src[0] ^ mac[0];
        block[1] = src[1] ^ mac[1];
        block[2] = src[2] ^ mac[2];
        block[3] = src[3] ^ mac[3];
        pCipher->Encrypt(mac, block);
        src += CcmMode::BLOCK_SIZE / sizeof(u32);
    }
}
} // namespace

// 0x0048389C | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::Initialize(const nn::crypto::BlockCipher& cipher, const void* pNonce, size_t nonceSize, size_t adataSize,
                                             size_t pdataSize, size_t macSize)
{
    m_pCipher = &cipher;
    m_BufferUsed = 0;
    m_MacSize = macSize;
    m_NonceSize = nonceSize;
    memset(m_Mac, 0, sizeof(m_Mac));
    memset(m_Counter, 0, sizeof(m_Counter));

    // the counter block: flags (the size of the counter - 1), the nonce, the counter (starting at 1)
    u8 counterSizeFlag = (BLOCK_SIZE - 2 - nonceSize) & 7;
    m_Counter[0] = counterSizeFlag;
    memcpy(m_Counter + 1, pNonce, nonceSize);
    CtrMode<BLOCK_SIZE>::IncrementCounter(m_Counter);

    // the first MAC block: flags, the nonce, the size of the plaintext (big endian)
    u32* block = reinterpret_cast<u32*>(m_Buffer);
    block[0] = 0;
    block[1] = 0;
    block[2] = 0;
    m_Buffer[0] = (adataSize != 0 ? FLAG_ADATA : 0) | ((macSize * 4 - 8) & 0x38) | counterSizeFlag;
    block[3] = __builtin_bswap32(pdataSize);
    memcpy(m_Buffer + 1, pNonce, nonceSize);
    ProcessCbcBlocks(m_Mac, m_Buffer, 1, m_pCipher);

    // the associated data starts with its size
    if (adataSize != 0) {
        if (adataSize < ADATA_SIZE_LIMIT) {
            m_Buffer[0] = adataSize >> 8;
            m_Buffer[1] = adataSize;
            m_BufferUsed = 2;
        } else {
            m_Buffer[0] = 0xFF;
            m_Buffer[1] = 0xFE;
            m_Buffer[2] = adataSize >> 24;
            m_Buffer[3] = adataSize >> 16;
            m_Buffer[4] = adataSize >> 8;
            m_Buffer[5] = adataSize;
            m_BufferUsed = 6;
        }
    }
}

// 0x004839E0 | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::GenerateMac(void* pMac, size_t macSize)
{
    // the counter 0 encrypts the MAC
    memset(m_Counter + 1 + m_NonceSize, 0, BLOCK_SIZE - 1 - m_NonceSize);
    u8 mac[BLOCK_SIZE];
    CtrMode<BLOCK_SIZE>::ProcessBlocks(mac, m_Counter, m_Mac, 1, *m_pCipher);
    memcpy(pMac, mac, static_cast<size_t>(m_MacSize) < macSize ? m_MacSize : macSize);
}

// 0x00483A4C | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::UpdateAdata(const void* pData, size_t size)
{
    const u8* src = static_cast<const u8*>(pData);
    size_t rest = size;
    const nn::crypto::BlockCipher* pCipher = m_pCipher;
    if (m_BufferUsed > 0) {
        size_t copySize = BLOCK_SIZE - m_BufferUsed;
        if (copySize > size) {
            copySize = size;
        }
        memcpy(m_Buffer + m_BufferUsed, src, copySize);
        src += copySize;
        rest -= copySize;
        m_BufferUsed += copySize;
        if (static_cast<size_t>(m_BufferUsed) < BLOCK_SIZE) {
            return;
        }
        ProcessCbcBlocks(m_Mac, m_Buffer, 1, pCipher);
        m_BufferUsed = 0;
    }
    int blockCount = rest / BLOCK_SIZE;
    ProcessCbcBlocks(m_Mac, src, blockCount, pCipher);
    size_t tail = rest - blockCount * BLOCK_SIZE;
    if (tail != 0) {
        memcpy(m_Buffer, src + blockCount * BLOCK_SIZE, tail);
    }
    m_BufferUsed = tail;
}

// 0x00483B0C | fefates:bytes [tier B]
size_t nn::crypto::detail::CcmMode::UpdateCdata(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize)
{
    u8* dst = static_cast<u8*>(pDst);
    const u8* src = static_cast<const u8*>(pSrc);
    size_t rest = srcSize;
    size_t written = 0;
    const nn::crypto::BlockCipher* pCipher = m_pCipher;
    if (m_BufferUsed > 0) {
        size_t copySize = BLOCK_SIZE - m_BufferUsed;
        if (copySize > srcSize) {
            copySize = srcSize;
        }
        memcpy(m_Buffer + m_BufferUsed, src, copySize);
        src += copySize;
        rest -= copySize;
        m_BufferUsed += copySize;
        if (static_cast<size_t>(m_BufferUsed) < BLOCK_SIZE) {
            return 0;
        }
        if (dstSize < BLOCK_SIZE) {
            return BLOCK_SIZE;
        }
        // the MAC is over the plaintext
        CtrMode<BLOCK_SIZE>::ProcessBlocks(dst, m_Counter, m_Buffer, 1, *pCipher);
        ProcessCbcBlocks(m_Mac, dst, 1, pCipher);
        written = m_BufferUsed;
        m_BufferUsed = 0;
        dst += written;
        dstSize -= written;
    }
    int blockCount = rest / BLOCK_SIZE;
    if (blockCount > static_cast<int>(dstSize / BLOCK_SIZE)) {
        return blockCount * BLOCK_SIZE;
    }
    CtrMode<BLOCK_SIZE>::ProcessBlocks(dst, m_Counter, src, blockCount, *pCipher);
    ProcessCbcBlocks(m_Mac, dst, blockCount, pCipher);
    size_t tail = rest - blockCount * BLOCK_SIZE;
    written += blockCount * BLOCK_SIZE;
    if (tail != 0) {
        memcpy(m_Buffer, src + blockCount * BLOCK_SIZE, tail);
    }
    m_BufferUsed = tail;
    return written;
}

// 0x00483C5C | fefates:bytes [tier B]
size_t nn::crypto::detail::CcmMode::UpdatePdata(void* pDst, size_t dstSize, const void* pSrc, size_t srcSize)
{
    u8* dst = static_cast<u8*>(pDst);
    const u8* src = static_cast<const u8*>(pSrc);
    size_t rest = srcSize;
    size_t written = 0;
    const nn::crypto::BlockCipher* pCipher = m_pCipher;
    if (m_BufferUsed > 0) {
        size_t copySize = BLOCK_SIZE - m_BufferUsed;
        if (copySize > srcSize) {
            copySize = srcSize;
        }
        memcpy(m_Buffer + m_BufferUsed, src, copySize);
        src += copySize;
        rest -= copySize;
        m_BufferUsed += copySize;
        if (static_cast<size_t>(m_BufferUsed) < BLOCK_SIZE) {
            return 0;
        }
        if (dstSize < BLOCK_SIZE) {
            return BLOCK_SIZE;
        }
        ProcessCbcBlocks(m_Mac, m_Buffer, 1, pCipher);
        CtrMode<BLOCK_SIZE>::ProcessBlocks(dst, m_Counter, m_Buffer, 1, *pCipher);
        written = m_BufferUsed;
        m_BufferUsed = 0;
        dst += written;
        dstSize -= written;
    }
    int blockCount = rest / BLOCK_SIZE;
    if (blockCount > static_cast<int>(dstSize / BLOCK_SIZE)) {
        return blockCount * BLOCK_SIZE;
    }
    ProcessCbcBlocks(m_Mac, src, blockCount, pCipher);
    CtrMode<BLOCK_SIZE>::ProcessBlocks(dst, m_Counter, src, blockCount, *pCipher);
    size_t tail = rest - blockCount * BLOCK_SIZE;
    written += blockCount * BLOCK_SIZE;
    if (tail != 0) {
        memcpy(m_Buffer, src + blockCount * BLOCK_SIZE, tail);
    }
    m_BufferUsed = tail;
    return written;
}

// 0x00483DB0 | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::UpdateAdataFinal()
{
    if (m_BufferUsed > 0) {
        memset(m_Buffer + m_BufferUsed, 0, BLOCK_SIZE - m_BufferUsed);
        ProcessCbcBlocks(m_Mac, m_Buffer, 1, m_pCipher);
        m_BufferUsed = 0;
    }
}

// 0x00483E00 | fefates:bytes [tier B]
size_t nn::crypto::detail::CcmMode::UpdateCdataFinal(void* pDst, size_t dstSize)
{
    size_t used = m_BufferUsed;
    const nn::crypto::BlockCipher* pCipher = m_pCipher;
    if (used != 0) {
        u8 block[BLOCK_SIZE];
        CtrMode<BLOCK_SIZE>::ProcessBlocks(block, m_Counter, m_Buffer, 1, *pCipher);
        memset(block + m_BufferUsed, 0, BLOCK_SIZE - m_BufferUsed);
        ProcessCbcBlocks(m_Mac, block, 1, pCipher);
        memcpy(pDst, block, dstSize > used ? used : dstSize);
        m_BufferUsed = 0;
    }
    return used;
}

// 0x00483EA0 | fefates:bytes [tier B]
size_t nn::crypto::detail::CcmMode::UpdatePdataFinal(void* pDst, size_t dstSize)
{
    size_t used = m_BufferUsed;
    const nn::crypto::BlockCipher* pCipher = m_pCipher;
    if (used != 0) {
        u8 block[BLOCK_SIZE];
        memset(m_Buffer + used, 0, BLOCK_SIZE - used);
        ProcessCbcBlocks(m_Mac, m_Buffer, 1, pCipher);
        CtrMode<BLOCK_SIZE>::ProcessBlocks(block, m_Counter, m_Buffer, 1, *pCipher);
        memcpy(pDst, block, dstSize > used ? used : dstSize);
        m_BufferUsed = 0;
    }
    return used;
}

// 0x00483F44 | fefates:bytes [tier B]
void nn::crypto::detail::CcmMode::Finalize()
{
    memset(m_Buffer, 0, sizeof(m_Buffer));
    memset(m_Mac, 0, sizeof(m_Mac));
    memset(m_Counter, 0, sizeof(m_Counter));
    m_pCipher = 0;
    m_BufferUsed = 0;
    m_NonceSize = 0;
    m_MacSize = 0;
}

} // namespace detail
} // namespace crypto
} // namespace nn
