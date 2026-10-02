#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SequenceTrackAllocator.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// RTTI N2nw3snd8internal6driver25MmlSequenceTrackAllocatorE @ 0x008D0B40
// vtable 0x009034CC (vptr 0x009034D4), offset_to_top 0, 5 entries
class MmlSequenceTrackAllocator : public ::nw::snd::internal::driver::SequenceTrackAllocator
{
public:
    MmlSequenceTrackAllocator(); // ctor address unknown
    virtual void vf_0x00(); // 0x004D3C54 slot 0x00 | virtual slot, introduced by nw::snd::internal::driver::MmlSequenceTrackAllocator
    virtual void vf_0x04(); // 0x004D3C50 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::MmlSequenceTrackAllocator
    virtual void AllocTrack(nw::snd::internal::driver::SequenceSoundPlayer*); // 0x004D3BDC slot 0x08 | nintendogs:bytes-fuzzy
    virtual void FreeTrack(nw::snd::internal::driver::SequenceTrack*); // 0x004D3C14 slot 0x0C | fefates:bytes
    virtual void GetAllocatableTrackCount() const; // 0x0074309C slot 0x10 | slot vf_0x10 of nw::snd::internal::driver::MmlSequenceTrackAllocator
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
