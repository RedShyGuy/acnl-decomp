#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// The memory of the stream channels: count blocks of the same size, a bit per block (the class and
// function names are from the reference symbols, the members are ours). The block of a channel holds
// the buffer blocks of the stream (StreamSoundPlayer).
class StreamBufferPool
{
public:
    // the blocks at most (Initialize clears one word of flags)
    static const int BLOCK_MAX = 32; // (name is ours)

    void Initialize(void* pBuffer, u32 size, int blockCount); // 0x004CE634 | nintendogs:bytes [tier A]
    void Finalize(); // 0x004CE76C (name is ours)
    // a free block (NULL: none)
    void* Alloc(); // 0x004CE6B8 | nintendogs:bytes [tier A]
    void Free(void* pBlock); // 0x004CE678 | nintendogs:bytes [tier A]

    void* m_pBuffer;                   // 0x00
    u32 m_Size;                        // 0x04
    u32 m_BlockSize;                   // 0x08 a multiple of 32
    int m_BlockCount;                  // 0x0C
    int m_AllocCount;                  // 0x10
    u8 m_AllocFlags[BLOCK_MAX / 8];    // 0x14 a bit per block
};
ASSERT_SIZE(StreamBufferPool, 0x18);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
