#include "nw/snd/internal/snd_StreamSoundFile.h"
#include "nw/snd/internal/snd_StreamSoundFileReader.h"

namespace nw {
namespace snd {
namespace internal {
// 0x00740A98 | fefates:bytes [tier B]
void nw::snd::internal::StreamSoundFileReader::ReadStreamSoundInfo(nw::snd::internal::StreamSoundFile::StreamSoundInfo*) const
{
}

// 0x00740B80 | fefates:bytes [tier B]
void nw::snd::internal::StreamSoundFileReader::ReadStreamTrackInfo(nw::snd::internal::StreamSoundFileReader::TrackInfo*, int) const
{
}

// 0x00740C24 | fefates:bytes [tier B]
void nw::snd::internal::StreamSoundFileReader::IsTrackInfoAvailable() const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
