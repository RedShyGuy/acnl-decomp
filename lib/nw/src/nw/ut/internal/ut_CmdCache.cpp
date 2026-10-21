#include <cstring>

#include "nw/ut/internal/ut_CmdCache.h"

namespace nw {
namespace ut {
namespace internal {
// 0x0048BAA0 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
void nw::ut::internal::CmdCache::Add(const u32* pCommands, u32 size)
{
    std::memcpy(m_pBuffer + m_Size, pCommands, size);
    m_Size += size;
}

// 0x0048BACC | nintendogs:bytes [confirmed by mk7dlp] [tier A]
void nw::ut::internal::CmdCache::Init(void* pBuffer, u32 size, bool flag)
{
    u8* pStart = reinterpret_cast<u8*>((reinterpret_cast<uptr>(pBuffer) + 3) & ~3);
    m_pBuffer = pStart;
    m_Flag = flag;
    m_Size = 0;
    m_Capacity = (static_cast<u8*>(pBuffer) + size - pStart) & ~7;
}

// 0x0048BAF8 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
void nw::ut::internal::CmdCache::RoundUp(u8 alignment)
{
    u32 size = (m_Size + alignment - 1) & ~(alignment - 1);
    std::memset(m_pBuffer + m_Size, 0, size - m_Size);
    m_Size = size;
}

// 0x0048BB2C (name is ours)
nw::ut::internal::CmdCache::CmdCache() : m_pBuffer(nullptr), m_Size(0), m_Capacity(0), m_Flag(false)
{
}

} // namespace internal
} // namespace ut
} // namespace nw
