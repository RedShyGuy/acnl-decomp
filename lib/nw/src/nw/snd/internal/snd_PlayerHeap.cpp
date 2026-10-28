#include "nw/snd/internal/snd_PlayerHeap.h"

#include "nw/ut/ut_Api.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C5E0C slot 0x08 | nintendogs:bytes
void* nw::snd::internal::PlayerHeap::Alloc(u32 size)
{
    u8* p = m_pAllocAddress;
    if (m_pEndAddress < p + size) {
        return nullptr;
    }
    m_pAllocAddress = ut::RoundUp(p + size, ALIGNMENT);
    return p;
}

// 0x004C5E38 | nintendogs:bytes [tier A]
void nw::snd::internal::PlayerHeap::Clear()
{
    m_pAllocAddress = m_pStartAddress;
}

// 0x004C5E44 | nintendogs:bytes [tier A]
bool nw::snd::internal::PlayerHeap::Create(void* pMemory, u32 size)
{
    u8* pEnd = static_cast<u8*>(pMemory) + size;
    u8* pStart = ut::RoundUp(static_cast<u8*>(pMemory), ALIGNMENT);
    if (pStart > pEnd) {
        return false;
    }
    m_pStartAddress = pStart;
    m_pEndAddress = pEnd;
    m_pAllocAddress = pStart;
    return true;
}

// 0x004C5E70 | fefates:bytes [tier B]
nw::snd::internal::PlayerHeap::PlayerHeap()
    : m_pSound(nullptr), m_pPlayer(nullptr), m_pStartAddress(nullptr), m_pEndAddress(nullptr), m_pAllocAddress(nullptr), m_State(STATE_CONSTRUCTED)
{
}

// 0x004C5EC4 slot 0x00 | mk7dlp:callseq (symbols.json: TaskThread::ThreadProc)
// 0x004C5EA4 (deleting dtor)
nw::snd::internal::PlayerHeap::~PlayerHeap()
{
    m_pAllocAddress = nullptr;
    m_State = STATE_DESTRUCTED;
}

} // namespace internal
} // namespace snd
} // namespace nw
