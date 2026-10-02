#include "nw/snd/internal/snd_StreamSoundFile.h"
#include "nw/snd/internal/snd_StreamSoundPrefetchFileReader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0074135C | fefates:bytes [tier B]
void nw::snd::internal::StreamSoundPrefetchFileReader::ReadStreamSoundInfo(nw::snd::internal::StreamSoundFile::StreamSoundInfo*) const
{
}

// 0x00741384 | fefates:bytes [tier B]
void nw::snd::internal::StreamSoundPrefetchFileReader::ReadPrefetchDataInfo(nw::snd::internal::StreamSoundPrefetchFileReader::PrefetchDataInfo*, int) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
