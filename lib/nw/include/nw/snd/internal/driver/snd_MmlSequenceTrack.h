#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SequenceTrack.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class MmlParser;

// A sequence track with MML commands (the class name and Parse are from the reference symbols, the member
// is ours).
// RTTI N2nw3snd8internal6driver16MmlSequenceTrackE @ 0x008D0AC0
// vtable 0x009033CC (vptr 0x009033D4), offset_to_top 0, 3 entries
class MmlSequenceTrack : public ::nw::snd::internal::driver::SequenceTrack
{
public:
    MmlSequenceTrack(); // 0x004CE60C | fefates:callgraph
    virtual ~MmlSequenceTrack(); // 0x004CCA88 slot 0x00

    virtual ParseResult Parse(bool doNoteOn); // 0x007425A4 slot 0x08 | fefates:bytes

    MmlParser* m_pParser; // 0x134
};
ASSERT_SIZE(MmlSequenceTrack, 0x138);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
