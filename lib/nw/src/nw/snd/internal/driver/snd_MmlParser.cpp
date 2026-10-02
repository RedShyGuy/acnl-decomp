#include "nw/snd/internal/driver/snd_MmlParser.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// ctor address unknown
nw::snd::internal::driver::MmlParser::MmlParser()
{
}

// 0x004D48F4 slot 0x00 | virtual slot, introduced by nw::snd::internal::driver::MmlParser
void nw::snd::internal::driver::MmlParser::vf_0x00()
{
}

// 0x004D48F0 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::MmlParser
void nw::snd::internal::driver::MmlParser::vf_0x04()
{
}

// 0x00741818 slot 0x08 | nintendogs:callseq
void nw::snd::internal::driver::MmlParser::CommandProc(nw::snd::internal::driver::MmlSequenceTrack*, unsigned, int, int) const
{
}

// 0x0074257C slot 0x0C | nintendogs:bytes
void nw::snd::internal::driver::MmlParser::NoteOnCommandProc(nw::snd::internal::driver::MmlSequenceTrack*, int, int, int, bool) const
{
}

// 0x004D48B8 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::MmlParser::ParseAllocTrack(const void*, unsigned, unsigned*)
{
}

// 0x00742B34 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::MmlParser::ReadArg(const unsigned char**, nw::snd::internal::driver::SequenceSoundPlayer*, nw::snd::internal::driver::SequenceTrack*, nw::snd::internal::driver::MmlParser::SeqArgType) const
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
