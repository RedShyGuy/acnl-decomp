#include "Audio/dAudioObjFurnitureBase.h"
#include "Audio/dAudioObjFurniture.h"
#include "Audio/dAudioObjFurnitureAudio.h"
#include "Audio/dAudioObjFurnitureInst.h"
#include "Audio/dAudioObjHaniwa.h"
#include "Audio/dAudioObjFurnitureOrgel.h"
#include "Audio/dAudioObjFurnitureInsect.h"

// 0x0094FD7C
bool AudioObjFurnitureBase::s_IsLoadSignalOn;

namespace {
// 0x00846304
AudioObjFurnitureBase* (* const s_FurnitureCreateFuncs[AudioObjFurnitureBase::KIND_NUM])(const SoundCreateArg*) = {
    &AudioObjFurniture::Create,
    &AudioObjFurnitureAudio::Create,
    &AudioObjFurnitureInst::Create,
    &AudioObjHaniwa::Create,
    &AudioObjFurnitureOrgel::Create,
    &AudioObjFurnitureInsect::Create,
};
} // namespace

// 0x00324FFC
AudioObjFurnitureBase::~AudioObjFurnitureBase()
{
    // empty (in the original too)
}

// 0x00324F04
void AudioObjFurnitureBase::Start(const nn::math::VEC3* world, u32 kind, bool isOn)
{
    // empty (in the original too)
}

// 0x00324F08
void AudioObjFurnitureBase::StartWithKind(u32 kind, const nn::math::VEC3* world)
{
    // empty (in the original too)
}

// 0x00324FD4
void AudioObjFurnitureBase::Update(const nn::math::VEC3* world, f32 frame)
{
    // empty (in the original too)
}

// 0x00324FC8
f32 AudioObjFurnitureBase::UpdateDance(const nn::math::VEC3* world)
{
    return 0.0f;
}

// 0x00324F0C
void AudioObjFurnitureBase::SetIsCueDisabled(bool isCueDisabled)
{
    // empty (in the original too)
}

// 0x00725824
bool AudioObjFurnitureBase::IsOffSoundPlaying()
{
    return false;
}

// 0x00324FF0
void AudioObjFurnitureBase::SetKind(u32 kind)
{
    // empty (in the original too)
}

// 0x00324FF4
void AudioObjFurnitureBase::TurnOn()
{
    // empty (in the original too)
}

// 0x00324F14
void AudioObjFurnitureBase::TurnOff()
{
    // empty (in the original too)
}

// 0x00324FC4
void AudioObjFurnitureBase::HoldSound(u32 soundId)
{
    // empty (in the original too)
}

// 0x00324F1C
void AudioObjFurnitureBase::SetNextIsOn()
{
    // empty (in the original too)
}

// 0x00324F80
void AudioObjFurnitureBase::LoadMusic(u32 song)
{
    // empty (in the original too)
}

// 0x00324FB4
bool AudioObjFurnitureBase::IsMusicLoaded()
{
    return true;
}

// 0x00324F7C
void AudioObjFurnitureBase::PlayMusic(s8 isFromStart, u32 milliSeconds)
{
    // empty (in the original too)
}

// 0x00324F6C
void AudioObjFurnitureBase::MusicSlot5C()
{
    // empty (in the original too)
}

// 0x00324F68
void AudioObjFurnitureBase::StopMusic(s32 fadeFrames)
{
    // empty (in the original too)
}

// 0x00324FA4
bool AudioObjFurnitureBase::IsMusicStopped()
{
    return true;
}

// 0x00324F8C
bool AudioObjFurnitureBase::IsMusicPlaying()
{
    return false;
}

// 0x00324FBC
u32 AudioObjFurnitureBase::GetMusicSong()
{
    return 0;
}

// 0x00324FAC
bool AudioObjFurnitureBase::IsMusicFinished()
{
    return true;
}

// 0x00324F58
void AudioObjFurnitureBase::Prepare()
{
    // empty (in the original too)
}

// 0x00324FE4
u8 AudioObjFurnitureBase::PlayNote()
{
    return 0;
}

// 0x00324F78
void AudioObjFurnitureBase::PlayNoteAt(s16 note)
{
    // empty (in the original too)
}

// 0x00324FE0
void AudioObjFurnitureBase::StartDance(bool isFree)
{
    // empty (in the original too)
}

// 0x00324FEC
void AudioObjFurnitureBase::StopDanceSound()
{
    // empty (in the original too)
}

// 0x00324F84
s32 AudioObjFurnitureBase::GetFramesToStart()
{
    return 0;
}

// 0x00324F18
void AudioObjFurnitureBase::PlaySong(u32 song, s32 wait)
{
    // empty (in the original too)
}

// 0x00324FF8
void AudioObjFurnitureBase::StopSong()
{
    // empty (in the original too)
}

// 0x00324F10
void AudioObjFurnitureBase::PauseSong(bool isPause)
{
    // empty (in the original too)
}

// 0x00324F70
bool AudioObjFurnitureBase::IsSongPlaying()
{
    return false;
}

// 0x00324F50
bool AudioObjFurnitureBase::IsSongPaused()
{
    return false;
}

// 0x00324F48
u32 AudioObjFurnitureBase::GetSongFrame()
{
    return 0;
}

// 0x00324F5C
f32 AudioObjFurnitureBase::GetFadeVolume(u32 song, u32 frame)
{
    return 0.0f;
}

// 0x00324FD8
bool AudioObjFurnitureBase::IsSinging()
{
    return false;
}

// 0x00324F20
AudioObjFurnitureBase* AudioObjFurnitureBase::Create(const SoundCreateArg* arg, u32 kind)
{
    return s_FurnitureCreateFuncs[kind](arg);
}

// 0x00324F30
void AudioObjFurnitureBase::Destroy(AudioObjFurnitureBase* obj)
{
    if (obj != NULL) {
        delete obj;
    }
}

// 0x00324F94
bool AudioObjFurnitureBase::IsLoadSignalOn()
{
    return s_IsLoadSignalOn;
}
