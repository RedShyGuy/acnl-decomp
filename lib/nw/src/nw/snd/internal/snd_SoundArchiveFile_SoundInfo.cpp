#include "nw/snd/internal/snd_SoundArchiveFile_SoundInfo.h"
#include "nw/snd/snd_SoundArchive.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the bits of the user parameters by index
// 0x008B6234
const u32 s_UserParamBits[4] = {31, 30, 29, 28};
} // namespace

// 0x0013FE44 | nintendogs:bytes [tier A]
u32 nw::snd::internal::SoundArchiveFile::SoundInfo::GetSoundType() const
{
    switch (detailInfo.typeId) {
    case TYPE_STREAM_SOUND_INFO:
        return SoundArchive::SOUND_TYPE_STRM;
    case TYPE_WAVE_SOUND_INFO:
        return SoundArchive::SOUND_TYPE_WAVE;
    case TYPE_SEQUENCE_SOUND_INFO:
        return SoundArchive::SOUND_TYPE_SEQ;
    default:
        return SoundArchive::SOUND_TYPE_INVALID;
    }
}

// 0x0013FE74 | nintendogs:callgraph [tier A]
const nw::snd::internal::SoundArchiveFile::WaveSoundInfo* nw::snd::internal::SoundArchiveFile::SoundInfo::GetWaveSoundInfo() const
{
    return static_cast<const WaveSoundInfo*>(GetReferedPointer(this, detailInfo));
}

// 0x0013FE80 (name after GetWaveSoundInfo)
const nw::snd::internal::SoundArchiveFile::SequenceSoundInfo* nw::snd::internal::SoundArchiveFile::SoundInfo::GetSequenceSoundInfo() const
{
    return static_cast<const SequenceSoundInfo*>(GetReferedPointer(this, detailInfo));
}

// 0x00141E68 | nintendogs:bytes [tier A]
u8 nw::snd::internal::SoundArchiveFile::SoundInfo::GetPanMode() const
{
    u32 value;
    if (!optionParams.GetValue(&value, OPTION_PAN)) {
        return 0;
    }
    return value & 0xFF;
}

// 0x00141E8C | nintendogs:bytes [tier A]
u8 nw::snd::internal::SoundArchiveFile::SoundInfo::GetPanCurve() const
{
    u32 value;
    if (!optionParams.GetValue(&value, OPTION_PAN)) {
        return 0;
    }
    return (value >> 8) & 0xFF;
}

// 0x00141EB8 | nintendogs:bytes [tier A]
bool nw::snd::internal::SoundArchiveFile::SoundInfo::IsFrontBypass() const
{
    u32 value;
    if (!optionParams.GetValue(&value, OPTION_FRONT_BYPASS)) {
        return false;
    }
    return value & 1;
}

// 0x00141EDC | nintendogs:bytes [tier A]
u8 nw::snd::internal::SoundArchiveFile::SoundInfo::GetActorPlayerId() const
{
    u32 value;
    if (!optionParams.GetValue(&value, OPTION_PLAYER)) {
        return 0;
    }
    return (value >> 8) & 0xFF;
}

// 0x00141F08 | nintendogs:bytes [tier A]
u8 nw::snd::internal::SoundArchiveFile::SoundInfo::GetPlayerPriority() const
{
    u32 value;
    if (!optionParams.GetValue(&value, OPTION_PLAYER)) {
        return 64;
    }
    return value & 0xFF;
}

// 0x007403A0 | fefates:bytes [tier B]
bool nw::snd::internal::SoundArchiveFile::SoundInfo::ReadUserParam(int index, u32& value) const
{
    return optionParams.GetValue(&value, s_UserParamBits[index]);
}

// 0x007403C4 | nintendogs:bytes [tier A]
const void* nw::snd::internal::SoundArchiveFile::SoundInfo::GetSound3DInfo() const
{
    u32 offset;
    if (!optionParams.GetValue(&offset, OPTION_SOUND_3D)) {
        return nullptr;
    }
    return reinterpret_cast<const u8*>(this) + offset;
}

// 0x007403EC | nintendogs:bytes [tier C]
const nw::snd::internal::SoundArchiveFile::StreamSoundInfo* nw::snd::internal::SoundArchiveFile::SoundInfo::GetStreamSoundInfo() const
{
    return static_cast<const StreamSoundInfo*>(GetReferedPointer(this, detailInfo));
}

// 0x007403F8 | nintendogs:bytes [tier C]
const nw::snd::internal::SoundArchiveFile::StreamSoundInfoLegacy* nw::snd::internal::SoundArchiveFile::SoundInfo::GetStreamSoundInfoLegacy() const
{
    return static_cast<const StreamSoundInfoLegacy*>(GetReferedPointer(this, detailInfo));
}

} // namespace internal
} // namespace snd
} // namespace nw
