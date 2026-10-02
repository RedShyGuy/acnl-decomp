#include "nw/snd/internal/snd_StreamSoundFile.h"
#include "nw/snd/internal/snd_StreamSoundFileLoader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C9534 | fefates:bytes [tier B]
void nw::snd::internal::StreamSoundFileLoader::LoadFileHeader(nw::snd::internal::StreamSoundFileReader*, void*, unsigned long)
{
}

// 0x004C96D8 | fefates:bytes [tier B]
void nw::snd::internal::StreamSoundFileLoader::ReadSeekBlockData(unsigned short*, unsigned short*, int, int)
{
}

// 0x007409E8 | fefates:bytes [tier B]
void nw::snd::internal::StreamSoundFileLoader::ReadRegionInfo(nw::snd::internal::StreamSoundFile::RegionInfo*, unsigned int) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
