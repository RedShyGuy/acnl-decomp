#include "sead/seadAudioDeviceSoundArchiveBaseCtr.h"

#include "nw/snd/snd_FsSoundArchive.h"
#include "nw/snd/snd_RomSoundArchive.h"

namespace sead {
// 0x00829C84 slot 0x00 (name is ours)
template bool sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::FsSoundArchive>::isKindOf(const sead::RuntimeTypeInfo::Interface* typeInfo) const;
// 0x00829C38 slot 0x04 (name is ours)
template const sead::RuntimeTypeInfo::Interface* sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::FsSoundArchive>::getTypeInfo() const;
// 0x00829D84 slot 0x00 (name is ours)
template bool sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::RomSoundArchive>::isKindOf(const sead::RuntimeTypeInfo::Interface* typeInfo) const;
// 0x00829D38 slot 0x04 (name is ours)
template const sead::RuntimeTypeInfo::Interface* sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::RomSoundArchive>::getTypeInfo() const;
// 0x00813348 slot 0x08
// 0x008132D8 (deleting dtor)
// 0x00778F80 (thunk)
// 0x00778F10 (deleting thunk)
template sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::FsSoundArchive>::~AudioDeviceSoundArchiveBaseCtr();
// 0x0081344C slot 0x08
// 0x008133DC (deleting dtor)
// 0x0077905C (thunk)
// 0x00778FEC (deleting thunk)
template sead::AudioDeviceSoundArchiveBaseCtr<nw::snd::RomSoundArchive>::~AudioDeviceSoundArchiveBaseCtr();

ASSERT_SIZE(AudioDeviceSoundArchiveBaseCtr<nw::snd::FsSoundArchive>, 0x1AC);
ASSERT_SIZE(AudioDeviceSoundArchiveBaseCtr<nw::snd::RomSoundArchive>, 0x1AC);
} // namespace sead
