#include "Audio/dAudioObjDoor.h"
#include "Sound/sound_Extern.h"

// 0x00987AB0
SoundMelody AudioObjDoor::s_DoorChimeMelody;

// 0x002D655C (name is ours)
AudioObjDoor::AudioObjDoor() : m_IsStarted(false)
{
}

// 0x002D610C (name is ours)
void AudioObjDoor::Start(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative);
    m_IsStarted = true;
}

// 0x002D6534 (name is ours)
void AudioObjDoor::Stop()
{
    if (m_IsStarted) {
        m_Obj.SoundObjWithMelody::Stop();
        m_IsStarted = false;
    }
}

// 0x002D6314 (name is ours)
void AudioObjDoor::SetPosition(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.SoundObjWithMelody::SetPosition(world, &relative);
}

// 0x002D6144 (name is ours)
bool AudioObjDoor::IsPlaying(u32 soundId) const
{
    return m_Obj.m_HandleSet.m_Handles[0].GetId() == soundId;
}

// 0x002D6344 (name is ours)
sead::SoundHandle* AudioObjDoor::StartSound(u32 soundId)
{
    return m_Obj.SoundObj<1>::StartSound(soundId);
}

// 0x002D6164 (name is ours)
void AudioObjDoor::PlayDoorChime(u32 soundId, const u8* packedMelody)
{
    UnpackMelody(packedMelody, &s_DoorChimeMelody);
    m_Obj.PlayDoorChime(soundId, s_DoorChimeMelody.notes);
}

// 0x002D6194 (name is ours)
void AudioObjDoor::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}
