#include "nw/snd/internal/snd_SoundArchiveFile_WaveSoundInfo.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013FD14 | nintendogs:bytes [tier A]
u8 nw::snd::internal::SoundArchiveFile::WaveSoundInfo::GetChannelPriority() const
{
    u32 value;
    if (!optionParams.GetValue(&value, 0)) {
        return 64;
    }
    return value & 0xFF;
}

// 0x0013FD3C | nintendogs:bytes [tier A]
u8 nw::snd::internal::SoundArchiveFile::WaveSoundInfo::GetIsReleasePriorityFix() const
{
    u32 value;
    if (!optionParams.GetValue(&value, 0)) {
        return false;
    }
    return (value >> 8) & 0xFF;
}

} // namespace internal
} // namespace snd
} // namespace nw
