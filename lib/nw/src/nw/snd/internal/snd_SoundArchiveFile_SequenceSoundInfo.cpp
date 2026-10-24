#include "nw/snd/internal/snd_SoundArchiveFile_SequenceSoundInfo.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013FD68 | nintendogs:bytes [tier A]
void nw::snd::internal::SoundArchiveFile::SequenceSoundInfo::GetBankIds(u32* pBankIds) const
{
    const BinaryTable<u32>* pTable = static_cast<const BinaryTable<u32>*>(GetReferedPointer(this, bankIdTable));
    for (u32 i = 0; i < 4; ++i) {
        pBankIds[i] = i < pTable->count ? pTable->items[i] : SoundArchive::INVALID_ID;
    }
}

// 0x0013FDC8 | nintendogs:bytes [tier A]
u32 nw::snd::internal::SoundArchiveFile::SequenceSoundInfo::GetStartOffset() const
{
    u32 value;
    if (!optionParams.GetValue(&value, 0)) {
        return 0;
    }
    return value;
}

// 0x0013FDE8 | nintendogs:bytes [tier A]
u8 nw::snd::internal::SoundArchiveFile::SequenceSoundInfo::GetChannelPriority() const
{
    u32 value;
    if (!optionParams.GetValue(&value, 1)) {
        return 64;
    }
    return value & 0xFF;
}

// 0x0013FE10 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFile::SequenceSoundInfo::IsReleasePriorityFix() const
{
    u32 value;
    if (!optionParams.GetValue(&value, 1)) {
        return false;
    }
    return ((value >> 8) & 0xFF) != 0;
}

} // namespace internal
} // namespace snd
} // namespace nw
