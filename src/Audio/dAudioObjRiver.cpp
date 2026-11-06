#include "Audio/dAudioObjRiver.h"
#include "Sound/sound_Extern.h"
#include "sead/seadNew.h"

// 0x00211168
AudioObjFgBase* AudioObjRiver::Create(const SoundCreateArg* arg)
{
    return new (arg->heap, 4) AudioObjRiver();
}

// 0x00211184
AudioObjRiver::AudioObjRiver() : AudioObjFgBase(0)
{
}

// 0x002111CC
// 0x002111AC (deleting dtor)
AudioObjRiver::~AudioObjRiver()
{
    // (only the members are destroyed)
}

// 0x002110C8
void AudioObjRiver::Start(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative);
    m_IsStarted = true;
}

// 0x00211110
void AudioObjRiver::SetPosition(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.SetPosition(world, &relative);
}

// 0x00211140
void AudioObjRiver::Stop()
{
    if (m_IsStarted) {
        m_Obj.Stop();
        m_IsStarted = false;
    }
}

// 0x00211100
void AudioObjRiver::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x00211108
s8 AudioObjRiver::IsIgnoreMaster()
{
    return m_Obj.m_IsIgnoreMaster;
}
