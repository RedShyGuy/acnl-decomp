#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SequenceTrack.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// RTTI N2nw3snd8internal6driver16MmlSequenceTrackE @ 0x008D0AC0
// vtable 0x009033CC (vptr 0x009033D4), offset_to_top 0, 3 entries
class MmlSequenceTrack : public ::nw::snd::internal::driver::SequenceTrack
{
public:
    MmlSequenceTrack(); // ctor candidate(s) 0x004CE60C (unverified)
    virtual ~MmlSequenceTrack(); // 0x004CCA88 slot 0x00 | slot vf_0x00 of nw::snd::internal::driver::SequenceTrack
    virtual void vf_0x04(); // 0x004CE624 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::SequenceTrack
    virtual void Parse(bool); // 0x007425A4 slot 0x08 | fefates:bytes
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
