#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
class StreamSoundFileReader
{
public:
    struct TrackInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void ReadStreamSoundInfo(nw::snd::internal::StreamSoundFile::StreamSoundInfo*) const; // 0x00740A98 | fefates:bytes [tier B]
    void ReadStreamTrackInfo(nw::snd::internal::StreamSoundFileReader::TrackInfo*, int) const; // 0x00740B80 | fefates:bytes [tier B]
    void IsTrackInfoAvailable() const; // 0x00740C24 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
