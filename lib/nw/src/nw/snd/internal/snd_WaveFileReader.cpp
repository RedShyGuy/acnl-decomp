#include "nw/snd/internal/snd_WaveFileReader.h"

namespace nw {
namespace snd {
namespace internal {
// TODO: default ctor added so derived stubs compile - may not exist
nw::snd::internal::WaveFileReader::WaveFileReader()
{
}

// 0x004C81EC | nintendogs:bytes [tier A]
void nw::snd::internal::WaveFileReader::GetSampleFormat(unsigned char)
{
}

// 0x004C8200 | fefates:bytes [tier B]
nw::snd::internal::WaveFileReader::WaveFileReader(const void*, signed char)
{
}

// 0x0073F86C | nintendogs:callseq [tier A]
void nw::snd::internal::WaveFileReader::ReadWaveInfo(nw::snd::internal::WaveInfo*, const void*) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
