#include "nw/snd/internal/snd_StreamSoundPrefetchFile.h"

namespace nw {
namespace snd {
namespace internal {
// 0x00741350 | fefates:bytes [tier B]
const void* nw::snd::internal::StreamSoundPrefetchFile::PrefetchData::GetPrefetchSample() const
{
    return GetReferedPointer(this, sampleReference);
}

} // namespace internal
} // namespace snd
} // namespace nw
