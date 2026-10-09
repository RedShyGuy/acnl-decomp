#include "nn/crypto/crypto_ShaBlock512BitContext.h"
#include "nn/nstd/nstd_String.h"

namespace nn {
namespace crypto {
namespace {
// the padding: 0x80, then zeros
// 0x00983730
u8 s_Padding[ShaBlock512BitContext::BLOCK_SIZE] = {0x80};

// the length in bits is the last 8 bytes of the last block
const u32 LENGTH_OFFSET = ShaBlock512BitContext::BLOCK_SIZE - sizeof(u64);
} // namespace

// 0x0014375C | nintendogs:bytes [tier A]
void nn::crypto::ShaBlock512BitContext::AddPadding()
{
    u32 length[2];
    length[1] = __builtin_bswap32((m_BlockCountLow << 9) + (m_BlockUsed << 3));
    length[0] = __builtin_bswap32((m_BlockCountHigh << 9) + (m_BlockCountLow >> 23));
    Update(s_Padding, m_BlockUsed < LENGTH_OFFSET ? LENGTH_OFFSET - m_BlockUsed : BLOCK_SIZE + LENGTH_OFFSET - m_BlockUsed);
    Update(length, sizeof(length));
}

// 0x001437E4 | nintendogs:bytes [tier A]
void nn::crypto::ShaBlock512BitContext::Update(const void* pData, size_t size)
{
    const u8* p = static_cast<const u8*>(pData);
    while (size != 0) {
        size_t copySize = BLOCK_SIZE - m_BlockUsed;
        if (copySize > size) {
            copySize = size;
        }
        nnnstdMemCpy(m_Block + m_BlockUsed, p, copySize);
        p += copySize;
        size -= copySize;
        m_BlockUsed += copySize;
        if (m_BlockUsed >= BLOCK_SIZE) {
            ProcessBlock();
            m_BlockUsed = 0;
            if (++m_BlockCountLow == 0) {
                m_BlockCountHigh++;
            }
        }
    }
}

} // namespace crypto
} // namespace nn
