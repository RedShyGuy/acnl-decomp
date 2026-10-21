#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
namespace internal {
// A buffer that collects GPU commands (font::RectDrawer has two). Member names are ours.
class CmdCache
{
public:
    CmdCache(); // 0x0048BB2C (name is ours)

    // the buffer (rounded up to 4 bytes; the capacity is a multiple of 8)
    void Init(void* pBuffer, u32 size, bool flag); // 0x0048BACC | nintendogs:bytes [confirmed by mk7dlp] [tier A]
    // the commands are appended
    void Add(const u32* pCommands, u32 size); // 0x0048BAA0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
    // the size is zero padded to the alignment
    void RoundUp(u8 alignment); // 0x0048BAF8 | nintendogs:bytes [confirmed by mk7dlp] [tier A]

    u8* m_pBuffer;   // 0x0
    u32 m_Size;      // 0x4
    u32 m_Capacity;  // 0x8
    bool m_Flag;     // 0xC (the flag of Init; no user known)
};
ASSERT_SIZE(CmdCache, 0x10);
} // namespace internal
} // namespace ut
} // namespace nw
