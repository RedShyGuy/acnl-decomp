#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
class StreamSoundFileLoader
{
public:
    void LoadFileHeader(nw::snd::internal::StreamSoundFileReader*, void*, unsigned long); // 0x004C9534 | fefates:bytes [tier B]
    void ReadSeekBlockData(unsigned short*, unsigned short*, int, int); // 0x004C96D8 | fefates:bytes [tier B]
    void ReadRegionInfo(nw::snd::internal::StreamSoundFile::RegionInfo*, unsigned int) const; // 0x007409E8 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
