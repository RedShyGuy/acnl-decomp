#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
class StreamSoundPrefetchFileReader
{
public:
    struct PrefetchDataInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void ReadStreamSoundInfo(nw::snd::internal::StreamSoundFile::StreamSoundInfo*) const; // 0x0074135C | fefates:bytes [tier B]
    void ReadPrefetchDataInfo(nw::snd::internal::StreamSoundPrefetchFileReader::PrefetchDataInfo*, int) const; // 0x00741384 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
