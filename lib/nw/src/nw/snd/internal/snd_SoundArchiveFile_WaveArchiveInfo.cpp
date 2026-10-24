#include "nw/snd/internal/snd_SoundArchiveFile_WaveArchiveInfo.h"

namespace nw {
namespace snd {
namespace internal {
// 0x00141E48 | nintendogs:bytes [tier A]
u32 nw::snd::internal::SoundArchiveFile::WaveArchiveInfo::GetWaveCount() const
{
    u32 value;
    if (!optionParams.GetValue(&value, 1)) {
        return 0;
    }
    return value;
}

} // namespace internal
} // namespace snd
} // namespace nw
