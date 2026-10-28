#include "nw/snd/internal/driver/snd_MmlSequenceTrack.h"

#include "nw/snd/internal/driver/snd_MmlParser.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CE60C | fefates:callgraph
nw::snd::internal::driver::MmlSequenceTrack::MmlSequenceTrack()
{
    // (only the base and the vtable)
}

// 0x004CCA88 slot 0x00
// 0x004CE624 (deleting dtor)
nw::snd::internal::driver::MmlSequenceTrack::~MmlSequenceTrack()
{
    // empty (in the original too)
}

// 0x007425A4 slot 0x08 | fefates:bytes
nw::snd::internal::driver::SequenceTrack::ParseResult nw::snd::internal::driver::MmlSequenceTrack::Parse(bool doNoteOn)
{
    return m_pParser->Parse(this, doNoteOn);
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
