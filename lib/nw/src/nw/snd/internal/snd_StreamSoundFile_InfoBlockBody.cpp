#include "nw/snd/internal/snd_StreamSoundFile_InfoBlockBody.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0073FBB8 | nintendogs:bytes [tier A]
const nw::snd::internal::StreamSoundFile::TrackInfoTable* nw::snd::internal::StreamSoundFile::InfoBlockBody::GetTrackInfoTable() const
{
    if (trackInfoTable.typeId != TYPE_REFERENCE_TABLE) {
        return nullptr;
    }
    return static_cast<const TrackInfoTable*>(GetReferedPointer(this, trackInfoTable));
}

// 0x0073FBD4 | nintendogs:bytes [tier A]
const nw::snd::internal::StreamSoundFile::StreamSoundInfo* nw::snd::internal::StreamSoundFile::InfoBlockBody::GetStreamSoundInfo() const
{
    if (streamSoundInfo.typeId != TYPE_STREAM_SOUND_INFO) {
        return nullptr;
    }
    return static_cast<const StreamSoundInfo*>(GetReferedPointer(this, streamSoundInfo));
}

// 0x0073FBEC | nintendogs:bytes [tier A]
const nw::snd::internal::StreamSoundFile::ChannelInfoTable* nw::snd::internal::StreamSoundFile::InfoBlockBody::GetChannelInfoTable() const
{
    if (channelInfoTable.typeId != TYPE_REFERENCE_TABLE) {
        return nullptr;
    }
    return static_cast<const ChannelInfoTable*>(GetReferedPointer(this, channelInfoTable));
}

// 0x0073FC08 | nintendogs:bytes [tier A]
const nw::snd::internal::StreamSoundFile::TrackInfo* nw::snd::internal::StreamSoundFile::TrackInfoTable::GetTrackInfo(u32 index) const
{
    return static_cast<const TrackInfo*>(table.GetReferedItem(index, TYPE_TRACK_INFO));
}

// 0x0073FC3C (name after GetTrackInfo)
const nw::snd::internal::StreamSoundFile::ChannelInfo* nw::snd::internal::StreamSoundFile::ChannelInfoTable::GetChannelInfo(u32 index) const
{
    return static_cast<const ChannelInfo*>(table.GetReferedItem(index, TYPE_CHANNEL_INFO));
}

} // namespace internal
} // namespace snd
} // namespace nw
