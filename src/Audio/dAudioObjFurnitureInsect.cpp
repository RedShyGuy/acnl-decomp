#include "Audio/dAudioObjFurnitureInsect.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "sead/seadNew.h"

// 0x00333870
AudioObjFurnitureBase* AudioObjFurnitureInsect::Create(const SoundCreateArg* arg)
{
    return new (arg->heap, 4) AudioObjFurnitureInsect;
}

// 0x0033388C
AudioObjFurnitureInsect::AudioObjFurnitureInsect() : AudioObjFurnitureBase(5)
{
}

// 0x003338DC
// 0x003338B8 (deleting dtor)
AudioObjFurnitureInsect::~AudioObjFurnitureInsect()
{
    // (only the members are destroyed)
}

// 0x00333700
void AudioObjFurnitureInsect::Start(const nn::math::VEC3* world, u32 kind, bool isOn)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative, kind, isOn);
    m_IsStarted = true;
}

// 0x003337E8
void AudioObjFurnitureInsect::Update(const nn::math::VEC3* world, f32 frame)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Update(world, &relative, frame);
}

// 0x00333838
void AudioObjFurnitureInsect::Stop()
{
    if (m_IsStarted) {
        m_Obj.Stop();
        m_IsStarted = false;
    }
}

// 0x003337D0
void AudioObjFurnitureInsect::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x003337D8
s8 AudioObjFurnitureInsect::IsIgnoreMaster()
{
    return m_Obj.m_IsIgnoreMaster;
}

// 0x0033378C
void AudioObjFurnitureInsect::SetVolumeScale(f32 scale)
{
    m_Obj.m_VolumeScale = scale;
}

// 0x00333794
bool AudioObjFurnitureInsect::IsPlaying()
{
    for (s32 i = 0; i < 2; i++) {
        if (m_Obj.m_HandleSet.m_Handles[i].m_pSound != NULL) {
            return true;
        }
    }
    return false;
}

// 0x00333750
bool AudioObjFurnitureInsect::IsPlaying(u32 soundId)
{
    return m_Obj.m_HandleSet.m_Handles[0].GetId() == soundId || m_Obj.m_HandleSet.m_Handles[1].GetId() == soundId;
}

// 0x00333830
void AudioObjFurnitureInsect::StartSound(u32 soundId)
{
    m_Obj.StartSound(soundId);
}

// 0x00333860
void AudioObjFurnitureInsect::SetKind(u32 kind)
{
    m_Obj.SetKind(kind);
}

// 0x00333868
void AudioObjFurnitureInsect::TurnOn()
{
    m_Obj.TurnOn();
}

// 0x00333748
void AudioObjFurnitureInsect::TurnOff()
{
    m_Obj.TurnOff();
}

// 0x003337E0
void AudioObjFurnitureInsect::HoldSound(u32 soundId)
{
    m_Obj.HoldSound(soundId);
}

// 0x00333828
bool AudioObjFurnitureInsect::IsSinging()
{
    return m_Obj.m_IsSinging;
}
