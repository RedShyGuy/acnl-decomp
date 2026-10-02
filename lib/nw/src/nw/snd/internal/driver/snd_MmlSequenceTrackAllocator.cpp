#include "nw/snd/internal/driver/snd_SequenceTrackAllocator.h"
#include "nw/snd/internal/driver/snd_MmlSequenceTrackAllocator.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// ctor address unknown
nw::snd::internal::driver::MmlSequenceTrackAllocator::MmlSequenceTrackAllocator()
{
}

// 0x004D3C54 slot 0x00 | virtual slot, introduced by nw::snd::internal::driver::MmlSequenceTrackAllocator
void nw::snd::internal::driver::MmlSequenceTrackAllocator::vf_0x00()
{
}

// 0x004D3C50 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::MmlSequenceTrackAllocator
void nw::snd::internal::driver::MmlSequenceTrackAllocator::vf_0x04()
{
}

// 0x004D3BDC slot 0x08 | nintendogs:bytes-fuzzy
void nw::snd::internal::driver::MmlSequenceTrackAllocator::AllocTrack(nw::snd::internal::driver::SequenceSoundPlayer*)
{
}

// 0x004D3C14 slot 0x0C | fefates:bytes
void nw::snd::internal::driver::MmlSequenceTrackAllocator::FreeTrack(nw::snd::internal::driver::SequenceTrack*)
{
}

// 0x0074309C slot 0x10 | slot vf_0x10 of nw::snd::internal::driver::MmlSequenceTrackAllocator
void nw::snd::internal::driver::MmlSequenceTrackAllocator::GetAllocatableTrackCount() const
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
