#include "nw/snd/internal/snd_WaveSoundFileReader.h"

namespace nw {
namespace snd {
namespace internal {
// TODO: default ctor added so derived stubs compile - may not exist
nw::snd::internal::WaveSoundFileReader::WaveSoundFileReader()
{
}

// 0x0013C310 | nintendogs:callgraph [tier A]
nw::snd::internal::WaveSoundFileReader::WaveSoundFileReader(const void*)
{
}

// 0x0013D44C | nintendogs:bytes [tier A]
void nw::snd::internal::WaveSoundFileReader::ReadNoteInfo(nw::snd::internal::WaveSoundNoteInfo*, unsigned, unsigned) const
{
}

// 0x007408C0 | nintendogs:callseq [tier A]
void nw::snd::internal::WaveSoundFileReader::ReadWaveSoundInfo(nw::snd::internal::WaveSoundInfo*, unsigned) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
