#include "nw/snd/internal/snd_SoundArchiveFile_PlayerInfo.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0073FE64 | nintendogs:bytes [tier A]
u32 nw::snd::internal::SoundArchiveFile::PlayerInfo::GetPlayerHeapSize() const
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
