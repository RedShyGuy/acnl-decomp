#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
class StreamDataInfoDetail
{
public:
    void SetStreamSoundInfo(const nw::snd::internal::StreamSoundFile::StreamSoundInfo&); // 0x004C9304 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
