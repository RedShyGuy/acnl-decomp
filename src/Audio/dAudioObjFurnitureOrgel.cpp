#include "Audio/dAudioObjFurnitureOrgel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "sead/seadNew.h"

// 0x0032DFC4
AudioObjFurnitureBase* AudioObjFurnitureOrgel::Create(const SoundCreateArg* arg)
{
    return new (arg->heap, 4) AudioObjFurnitureOrgel;
}

// 0x0032DFE0
AudioObjFurnitureOrgel::AudioObjFurnitureOrgel() : AudioObjFurnitureBase(0), m_IsActive(false)
{
}

// 0x0032E030
// 0x0032E00C (deleting dtor)
AudioObjFurnitureOrgel::~AudioObjFurnitureOrgel()
{
    // (only the members are destroyed)
}

// 0x0032DE24
void AudioObjFurnitureOrgel::Start(const nn::math::VEC3* world, u32 kind, bool isOn)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.SoundObjFurniture::Start(world, &relative, kind, isOn);
    m_IsActive = true;
}

// 0x0032DF44
void AudioObjFurnitureOrgel::Update(const nn::math::VEC3* world, f32 frame)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Update(world, &relative, frame);
}

// 0x0032DF8C
void AudioObjFurnitureOrgel::Stop()
{
    if (m_IsActive) {
        m_Obj.Stop();
        m_IsActive = false;
    }
}

// 0x0032DF2C
void AudioObjFurnitureOrgel::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x0032DF34
s8 AudioObjFurnitureOrgel::IsIgnoreMaster()
{
    return m_Obj.m_IsIgnoreMaster;
}

// 0x0032DEE8
void AudioObjFurnitureOrgel::SetVolumeScale(f32 scale)
{
    m_Obj.m_VolumeScale = scale;
}

// 0x0032DEF0
bool AudioObjFurnitureOrgel::IsPlaying()
{
    for (s32 i = 0; i < 2; i++) {
        if (m_Obj.m_HandleSet.m_Handles[i].m_pSound != NULL) {
            return true;
        }
    }
    return false;
}

// 0x0032DEAC
bool AudioObjFurnitureOrgel::IsPlaying(u32 soundId)
{
    return m_Obj.m_HandleSet.m_Handles[0].GetId() == soundId || m_Obj.m_HandleSet.m_Handles[1].GetId() == soundId;
}

// 0x0032DF84
void AudioObjFurnitureOrgel::StartSound(u32 soundId)
{
    m_Obj.StartSound(soundId);
}

// 0x0032DFB4
void AudioObjFurnitureOrgel::SetKind(u32 kind)
{
    m_Obj.SetKind(kind);
}

// 0x0032DFBC
void AudioObjFurnitureOrgel::TurnOn()
{
    m_Obj.TurnOn();
}

// 0x0032DE6C
void AudioObjFurnitureOrgel::TurnOff()
{
    m_Obj.TurnOff();
}

// 0x0032DF3C
void AudioObjFurnitureOrgel::HoldSound(u32 soundId)
{
    m_Obj.HoldSound(soundId);
}

// 0x0033248C
void AudioObjFurnitureOrgel::PlaySong(u32 song, s32 wait)
{
    m_Obj.Play(song, wait);
}

// 0x00332AEC
void AudioObjFurnitureOrgel::StopSong()
{
    m_Obj.StopSong();
}

// 0x00332368
void AudioObjFurnitureOrgel::PauseSong(bool isPause)
{
    m_Obj.Pause(isPause);
}

// 0x0032DE94
bool AudioObjFurnitureOrgel::IsSongPlaying()
{
    return m_Obj.m_State == SoundObjFurnitureOrgel::STATE_PLAYING ||
           m_Obj.m_State == SoundObjFurnitureOrgel::STATE_WAITING;
}

// 0x0032DE7C
bool AudioObjFurnitureOrgel::IsSongPaused()
{
    return m_Obj.m_State == SoundObjFurnitureOrgel::STATE_PAUSED ||
           m_Obj.m_State == SoundObjFurnitureOrgel::STATE_WAITING_PAUSED;
}

// 0x0032DE74
u32 AudioObjFurnitureOrgel::GetSongFrame()
{
    return m_Obj.m_Frame;
}

// 0x007264DC
f32 AudioObjFurnitureOrgel::GetFadeVolume(u32 song, u32 frame)
{
    return m_Obj.GetFadeVolume(song, frame);
}
