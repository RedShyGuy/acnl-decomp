#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class WaveSoundFileReader
{
public:
    WaveSoundFileReader(); // TODO: default ctor added so derived stubs compile - may not exist
    WaveSoundFileReader(const void*); // 0x0013C310 | nintendogs:callgraph [tier A]
    void ReadNoteInfo(nw::snd::internal::WaveSoundNoteInfo*, unsigned, unsigned) const; // 0x0013D44C | nintendogs:bytes [tier A]
    void ReadWaveSoundInfo(nw::snd::internal::WaveSoundInfo*, unsigned) const; // 0x007408C0 | nintendogs:callseq [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
