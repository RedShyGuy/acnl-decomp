#include "nw/snd/internal/snd_SoundArchiveFile_StreamSoundInfo.h"

namespace nw {
namespace snd {
namespace internal {
// 0x00740184 | nintendogs:bytes [tier C]
const u32* nw::snd::internal::SoundArchiveFile::StreamSoundInfo::GetSendValue() const
{
    return static_cast<const u32*>(GetReferedPointer(this, sendValue));
}

// 0x00740190 | nintendogs:bytes [tier C]
const nw::snd::internal::BinaryReferenceTable* nw::snd::internal::SoundArchiveFile::StreamSoundInfo::GetTrackInfoTable() const
{
    if (trackInfoTable.typeId != TYPE_TABLE_EMBEDDING_TABLE) {
        return nullptr;
    }
    return static_cast<const BinaryReferenceTable*>(GetReferedPointer(this, trackInfoTable));
}

// 0x007401AC | nintendogs:bytes [tier B]
const nw::snd::internal::SoundArchiveFile::StreamSoundInfo::StreamSoundExtension* nw::snd::internal::SoundArchiveFile::StreamSoundInfo::GetStreamSoundExtension() const
{
    if (!streamSoundExtension.IsValid() || streamSoundExtension.typeId != TYPE_STREAM_SOUND_EXTENSION) {
        return nullptr;
    }
    return static_cast<const StreamSoundExtension*>(GetReferedPointer(this, streamSoundExtension));
}

} // namespace internal
} // namespace snd
} // namespace nw
