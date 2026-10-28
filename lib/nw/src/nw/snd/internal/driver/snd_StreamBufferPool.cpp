#include "nw/snd/internal/driver/snd_StreamBufferPool.h"

#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CE634 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::StreamBufferPool::Initialize(void* pBuffer, u32 size, int blockCount)
{
    if (blockCount == 0) {
        return;
    }
    m_pBuffer = pBuffer;
    m_Size = size;
    m_BlockCount = blockCount;
    m_BlockSize = size / blockCount & ~31;
    m_AllocCount = 0;
    for (int i = 0; i < BLOCK_MAX / 8; i++) {
        m_AllocFlags[i] = 0;
    }
}

// 0x004CE678 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::StreamBufferPool::Free(void* pBlock)
{
    const u32 index = (static_cast<u8*>(pBlock) - static_cast<u8*>(m_pBuffer)) / m_BlockSize;
    m_AllocFlags[index / 8] &= ~(1 << (index % 8));
    m_AllocCount--;
}

// 0x004CE6B8 | nintendogs:bytes [tier A]
void* nw::snd::internal::driver::StreamBufferPool::Alloc()
{
    if (m_BlockCount <= m_AllocCount) {
        return nullptr;
    }
    const int byteCount = static_cast<int>(ut::RoundUp(m_BlockCount, 8)) / 8;
    for (int byteIndex = 0; byteIndex < byteCount; byteIndex++) {
        const u8 flags = m_AllocFlags[byteIndex];
        if (flags == 0xFF) {
            continue;
        }
        u8 mask = 1;
        for (int bitIndex = 0; bitIndex < 8; bitIndex++) {
            if ((flags & mask) == 0) {
                m_AllocFlags[byteIndex] |= mask;
                m_AllocCount++;
                return static_cast<u8*>(m_pBuffer) + m_BlockSize * (byteIndex * 8 + bitIndex);
            }
            mask <<= 1;
        }
    }
    return nullptr;
}

// 0x004CE76C (name is ours)
void nw::snd::internal::driver::StreamBufferPool::Finalize()
{
    m_pBuffer = nullptr;
    m_Size = 0;
    m_BlockSize = 0;
    m_BlockCount = 0;
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
