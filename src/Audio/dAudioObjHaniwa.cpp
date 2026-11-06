#include "Audio/dAudioObjHaniwa.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "sead/seadNew.h"

// 0x00250104
AudioObjFurnitureBase* AudioObjHaniwa::Create(const SoundCreateArg* arg)
{
    return new (arg->heap, 4) AudioObjHaniwa;
}

// 0x00250124
AudioObjHaniwa::AudioObjHaniwa() : AudioObjFurnitureBase(3)
{
}

// 0x00250170
// 0x0025014C (deleting dtor)
AudioObjHaniwa::~AudioObjHaniwa()
{
    // (only the members are destroyed)
}

// 0x0024FCF0
void AudioObjHaniwa::StartWithKind(u32 kind, const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(kind, world, &relative);
    m_IsStarted = true;
}

// 0x0024FDC4
f32 AudioObjHaniwa::UpdateDance(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    return m_Obj.UpdateDance(world, &relative);
}

// 0x002500DC
void AudioObjHaniwa::Stop()
{
    if (m_IsStarted) {
        m_Obj.Stop();
        m_IsStarted = false;
    }
}

// 0x0024FDB4
void AudioObjHaniwa::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x0024FDBC
s8 AudioObjHaniwa::IsIgnoreMaster()
{
    return m_Obj.m_IsIgnoreMaster;
}

// 0x0024FD70
void AudioObjHaniwa::SetVolumeScale(f32 scale)
{
    m_Obj.m_VolumeScale = scale;
}

// 0x0024FD78
bool AudioObjHaniwa::IsPlaying()
{
    for (s32 i = 0; i < 2; i++) {
        if (m_Obj.m_HandleSet.m_Handles[i].m_pSound != NULL) {
            return true;
        }
    }
    return false;
}

// 0x0024FD34
bool AudioObjHaniwa::IsPlaying(u32 soundId)
{
    return m_Obj.m_HandleSet.m_Handles[0].GetId() == soundId || m_Obj.m_HandleSet.m_Handles[1].GetId() == soundId;
}

// 0x0024FDF4
void AudioObjHaniwa::StartSound(u32 soundId)
{
    // (not in the stage of the dream)
    if (m_Obj.m_IsActive && SoundMgr::s_pInstance->m_State != 3) {
        m_Obj.StartSound(soundId);
    }
}

// 0x002777F0
void AudioObjHaniwa::Prepare()
{
    m_Obj.Register();
}

// 0x00277EF4
void AudioObjHaniwa::StartDance(bool isFree)
{
    m_Obj.StartDance(isFree);
}

// 0x0027808C
void AudioObjHaniwa::StopDanceSound()
{
    m_Obj.StopSound();
}

// 0x00277CDC
s32 AudioObjHaniwa::GetFramesToStart()
{
    return m_Obj.GetFramesToStart();
}
