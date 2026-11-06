#include "Audio/dAudioObjFurniture.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "sead/seadNew.h"

// 0x002C554C
AudioObjFurnitureBase* AudioObjFurniture::Create(const SoundCreateArg* arg)
{
    return new (arg->heap, 4) AudioObjFurniture;
}

// 0x002C556C
AudioObjFurniture::AudioObjFurniture() : AudioObjFurnitureBase(0)
{
}

// 0x002C55B4
// 0x002C5590 (deleting dtor)
AudioObjFurniture::~AudioObjFurniture()
{
    // (only the members are destroyed)
}

// 0x002C53F8
void AudioObjFurniture::Start(const nn::math::VEC3* world, u32 kind, bool isOn)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative, kind, isOn);
    m_IsStarted = true;
}

// 0x002C54E4
void AudioObjFurniture::Update(const nn::math::VEC3* world, f32 frame)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Update(world, &relative, frame);
}

// 0x002C5524
void AudioObjFurniture::Stop()
{
    if (m_IsStarted) {
        m_Obj.Stop();
        m_IsStarted = false;
    }
}

// 0x002C54D4
void AudioObjFurniture::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x002C54DC
s8 AudioObjFurniture::IsIgnoreMaster()
{
    return m_Obj.m_IsIgnoreMaster;
}

// 0x002C5490
void AudioObjFurniture::SetVolumeScale(f32 scale)
{
    m_Obj.m_VolumeScale = scale;
}

// 0x002C5498
bool AudioObjFurniture::IsPlaying()
{
    for (s32 i = 0; i < 2; i++) {
        if (m_Obj.m_HandleSet.m_Handles[i].m_pSound != NULL) {
            return true;
        }
    }
    return false;
}

// 0x002C5454
bool AudioObjFurniture::IsPlaying(u32 soundId)
{
    return m_Obj.m_HandleSet.m_Handles[0].GetId() == soundId || m_Obj.m_HandleSet.m_Handles[1].GetId() == soundId;
}

// 0x002D2C48
void AudioObjFurniture::StartSound(u32 soundId)
{
    m_Obj.StartSound(soundId);
}

// 0x002C5440
void AudioObjFurniture::SetIsCueDisabled(bool isCueDisabled)
{
    m_Obj.m_IsCueDisabled = isCueDisabled;
}

// 0x00720504
bool AudioObjFurniture::IsOffSoundPlaying()
{
    return m_Obj.IsOffSoundPlaying();
}

// 0x002D2F64
void AudioObjFurniture::SetKind(u32 kind)
{
    m_Obj.SetKind(kind);
}

// 0x002D2FD8
void AudioObjFurniture::TurnOn()
{
    m_Obj.TurnOn();
}

// 0x002D25D4
void AudioObjFurniture::TurnOff()
{
    m_Obj.TurnOff();
}

// 0x002D2998
void AudioObjFurniture::HoldSound(u32 soundId)
{
    m_Obj.HoldSound(soundId);
}

// 0x002C5448
void AudioObjFurniture::SetNextIsOn()
{
    m_Obj.m_NextIsOn = true;
}
