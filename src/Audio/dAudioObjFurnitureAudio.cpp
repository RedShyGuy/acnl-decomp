#include "Audio/dAudioObjFurnitureAudio.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "sead/seadNew.h"

// 0x0032DD98
AudioObjFurnitureBase* AudioObjFurnitureAudio::Create(const SoundCreateArg* arg)
{
    return new (arg->heap, 4) AudioObjFurnitureAudio;
}

// 0x0032DDB4
AudioObjFurnitureAudio::AudioObjFurnitureAudio() : AudioObjFurnitureBase(1)
{
}

// 0x0032DE04
// 0x0032DDE0 (deleting dtor)
AudioObjFurnitureAudio::~AudioObjFurnitureAudio()
{
    // (only the members are destroyed)
}

// 0x0032DC24
void AudioObjFurnitureAudio::Start(const nn::math::VEC3* world, u32 kind, bool isOn)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative, kind, isOn);
    m_IsStarted = true;
}

// 0x0032DD18
void AudioObjFurnitureAudio::Update(const nn::math::VEC3* world, f32 frame)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Update(world, &relative, frame);
}

// 0x0032DD60
void AudioObjFurnitureAudio::Stop()
{
    if (m_IsStarted) {
        m_Obj.Stop();
        m_IsStarted = false;
    }
}

// 0x0032DD00
void AudioObjFurnitureAudio::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x0032DD08
s8 AudioObjFurnitureAudio::IsIgnoreMaster()
{
    return m_Obj.m_IsIgnoreMaster;
}

// 0x0032DCB0
void AudioObjFurnitureAudio::SetVolumeScale(f32 scale)
{
    m_Obj.m_VolumeScale = scale;
}

// 0x0032DCB8
bool AudioObjFurnitureAudio::IsPlaying()
{
    for (s32 i = 0; i < 2; i++) {
        if (m_Obj.m_HandleSet.m_Handles[i].m_pSound != NULL) {
            return true;
        }
    }
    return false;
}

// 0x0032DC74
bool AudioObjFurnitureAudio::IsPlaying(u32 soundId)
{
    return m_Obj.m_HandleSet.m_Handles[0].GetId() == soundId || m_Obj.m_HandleSet.m_Handles[1].GetId() == soundId;
}

// 0x0032DD58
void AudioObjFurnitureAudio::StartSound(u32 soundId)
{
    m_Obj.StartSound(soundId);
}

// 0x0032DD88
void AudioObjFurnitureAudio::SetKind(u32 kind)
{
    m_Obj.SetKind(kind);
}

// 0x0032DD90
void AudioObjFurnitureAudio::TurnOn()
{
    m_Obj.TurnOn();
}

// 0x0032DC6C
void AudioObjFurnitureAudio::TurnOff()
{
    m_Obj.TurnOff();
}

// 0x0032DD10
void AudioObjFurnitureAudio::HoldSound(u32 soundId)
{
    m_Obj.HoldSound(soundId);
}

// 0x003320C0
void AudioObjFurnitureAudio::LoadMusic(u32 song)
{
    m_Obj.LoadMusic(song);
}

// 0x0072648C
bool AudioObjFurnitureAudio::IsMusicLoaded()
{
    return m_Obj.IsMusicLoaded();
}

// 0x0033209C
void AudioObjFurnitureAudio::PlayMusic(s8 isFromStart, u32 milliSeconds)
{
    m_Obj.PlayMusic(isFromStart, milliSeconds);
}

// 0x00332054
void AudioObjFurnitureAudio::StopMusic(s32 fadeFrames)
{
    m_Obj.StopMusic(fadeFrames);
}

// 0x0072642C
bool AudioObjFurnitureAudio::IsMusicStopped()
{
    return m_Obj.IsMusicStopped();
}

// 0x00726408
bool AudioObjFurnitureAudio::IsMusicPlaying()
{
    return m_Obj.IsMusicPlaying();
}

// 0x007264BC
u32 AudioObjFurnitureAudio::GetMusicSong()
{
    return m_Obj.GetMusicSong();
}

// 0x00726458
bool AudioObjFurnitureAudio::IsMusicFinished()
{
    return m_Obj.IsMusicFinished();
}
