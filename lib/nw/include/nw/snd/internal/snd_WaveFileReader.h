#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class WaveFileReader
{
public:
    WaveFileReader(); // TODO: default ctor added so derived stubs compile - may not exist
    void GetSampleFormat(unsigned char); // 0x004C81EC | nintendogs:bytes [tier A]
    WaveFileReader(const void*, signed char); // 0x004C8200 | fefates:bytes [tier B]
    void ReadWaveInfo(nw::snd::internal::WaveInfo*, const void*) const; // 0x0073F86C | nintendogs:callseq [tier A]
};
} // namespace internal
} // namespace snd
} // namespace nw
