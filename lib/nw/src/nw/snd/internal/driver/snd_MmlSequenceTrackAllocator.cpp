#include "nw/snd/internal/driver/snd_MmlSequenceTrackAllocator.h"

#include <new>

#include "nw/snd/internal/driver/snd_MmlSequenceTrack.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004D3BDC slot 0x08 | nintendogs:bytes-fuzzy
nw::snd::internal::driver::SequenceTrack* nw::snd::internal::driver::MmlSequenceTrackAllocator::AllocTrack(SequenceSoundPlayer* pPlayer)
{
    void* pMemory = m_TrackPool.AllocImpl();
    if (pMemory == nullptr) {
        return nullptr;
    }
    MmlSequenceTrack* pTrack = new (pMemory) MmlSequenceTrack();
    if (pTrack == nullptr) {
        return nullptr;
    }
    pTrack->m_pSequenceSoundPlayer = pPlayer;
    pTrack->m_pParser = m_pParser;
    return pTrack;
}

// 0x004D3C14 slot 0x0C | fefates:bytes
void nw::snd::internal::driver::MmlSequenceTrackAllocator::FreeTrack(SequenceTrack* pTrack)
{
    pTrack->m_pSequenceSoundPlayer = nullptr;
    if (pTrack == nullptr) {
        return;
    }
    pTrack->~SequenceTrack();
    m_TrackPool.FreeImpl(pTrack);
}

// 0x004D3C54 slot 0x00
// 0x004D3C50 (deleting dtor)
nw::snd::internal::driver::MmlSequenceTrackAllocator::~MmlSequenceTrackAllocator()
{
    // empty (in the original too)
}

// 0x004D4A74 | fefates:callgraph [tier C]
u32 nw::snd::internal::driver::MmlSequenceTrackAllocator::Create(void* pMemory, unsigned long size)
{
    return m_TrackPool.CreateImpl(pMemory, size, sizeof(MmlSequenceTrack));
}

// 0x004D4AD8 | fefates:callgraph [tier C]
void nw::snd::internal::driver::MmlSequenceTrackAllocator::Destroy(void* pMemory, unsigned long size)
{
    m_TrackPool.DestroyImpl(pMemory, size);
}

// 0x0074309C slot 0x10
int nw::snd::internal::driver::MmlSequenceTrackAllocator::GetAllocatableTrackCount() const
{
    return m_TrackPool.CountImpl();
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
