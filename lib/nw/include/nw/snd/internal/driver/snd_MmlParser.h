#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// RTTI N2nw3snd8internal6driver9MmlParserE @ 0x008D0B58
// vtable 0x009034FC (vptr 0x00903504), offset_to_top 0, 4 entries
class MmlParser
{
public:
    struct SeqArgType { u32 _unknown; }; // TODO: real type unknown (placeholder)
    MmlParser(); // ctor address unknown
    virtual void vf_0x00(); // 0x004D48F4 slot 0x00 | virtual slot, introduced by nw::snd::internal::driver::MmlParser
    virtual void vf_0x04(); // 0x004D48F0 slot 0x04 | virtual slot, introduced by nw::snd::internal::driver::MmlParser
    virtual void CommandProc(nw::snd::internal::driver::MmlSequenceTrack*, unsigned, int, int) const; // 0x00741818 slot 0x08 | nintendogs:callseq
    virtual void NoteOnCommandProc(nw::snd::internal::driver::MmlSequenceTrack*, int, int, int, bool) const; // 0x0074257C slot 0x0C | nintendogs:bytes
    void ParseAllocTrack(const void*, unsigned, unsigned*); // 0x004D48B8 | nintendogs:bytes-fuzzy [tier A]
    void ReadArg(const unsigned char**, nw::snd::internal::driver::SequenceSoundPlayer*, nw::snd::internal::driver::SequenceTrack*, nw::snd::internal::driver::MmlParser::SeqArgType) const; // 0x00742B34 | nintendogs:callseq [tier A]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
