#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SequenceTrackAllocator.h"
#include "nw/snd/internal/snd_PoolImpl.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class MmlParser;

// The MML tracks in a buffer, all with one parser (the class and function names are from the reference
// symbols, the members are ours).
// RTTI N2nw3snd8internal6driver25MmlSequenceTrackAllocatorE @ 0x008D0B40
// vtable 0x009034CC (vptr 0x009034D4), offset_to_top 0, 5 entries
class MmlSequenceTrackAllocator : public ::nw::snd::internal::driver::SequenceTrackAllocator
{
public:
    // (inline; name is ours)
    explicit MmlSequenceTrackAllocator(MmlParser* pParser) : m_pParser(pParser) {}
    virtual ~MmlSequenceTrackAllocator(); // 0x004D3C54 slot 0x00

    virtual SequenceTrack* AllocTrack(SequenceSoundPlayer* pPlayer); // 0x004D3BDC slot 0x08 | nintendogs:bytes-fuzzy
    virtual void FreeTrack(SequenceTrack* pTrack); // 0x004D3C14 slot 0x0C | fefates:bytes
    virtual int GetAllocatableTrackCount() const; // 0x0074309C slot 0x10

    // the tracks in the memory; their count
    u32 Create(void* pMemory, unsigned long size); // 0x004D4A74 | fefates:callgraph [tier C]
    void Destroy(void* pMemory, unsigned long size); // 0x004D4AD8 | fefates:callgraph [tier C]

    MmlParser* m_pParser; // 0x4
    PoolImpl m_TrackPool; // 0x8 (MmlSequenceTrack)
};
ASSERT_SIZE(MmlSequenceTrackAllocator, 0xC);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
