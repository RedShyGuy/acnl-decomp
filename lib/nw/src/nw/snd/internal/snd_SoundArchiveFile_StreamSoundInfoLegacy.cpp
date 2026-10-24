#include "nw/snd/internal/snd_SoundArchiveFile_StreamSoundInfoLegacy.h"

namespace nw {
namespace snd {
namespace internal {
// 0x007402E8 | fefates:bytes [tier B]
u32 nw::snd::internal::SoundArchiveFile::StreamSoundInfoLegacy::GetLoopEndFrame() const
{
    u32 value;
    if (!optionParams.GetValue(&value, 2)) {
        return 0;
    }
    return value;
}

// 0x00740308 | fefates:bytes [tier B]
u32 nw::snd::internal::SoundArchiveFile::StreamSoundInfoLegacy::GetLoopStartFrame() const
{
    u32 value;
    if (!optionParams.GetValue(&value, 1)) {
        return 0;
    }
    return value;
}

// 0x00740328 | fefates:bytes [tier B]
u8 nw::snd::internal::SoundArchiveFile::StreamSoundInfoLegacy::GetStreamFileType() const
{
    u32 value;
    if (!optionParams.GetValue(&value, 0)) {
        return 1;
    }
    return value & 0xFF;
}

// 0x00740350 | nintendogs:bytes [tier C]
const nw::snd::internal::BinaryReferenceTable* nw::snd::internal::SoundArchiveFile::StreamSoundInfoLegacy::GetTrackInfoTable() const
{
    if (trackInfoTable.typeId != TYPE_TABLE_EMBEDDING_TABLE) {
        return nullptr;
    }
    return static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, trackInfoTable));
}

// 0x0074036C | fefates:bytes [tier B]
bool nw::snd::internal::SoundArchiveFile::StreamSoundInfoLegacy::IsLoop() const
{
    u32 value;
    if (!optionParams.GetValue(&value, 0)) {
        return false;
    }
    return ((value >> 8) & 0xFF) == 1;
}

} // namespace internal
} // namespace snd
} // namespace nw
