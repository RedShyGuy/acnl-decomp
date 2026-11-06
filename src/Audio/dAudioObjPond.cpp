#include "Audio/dAudioObjPond.h"
#include "Sound/sound_Extern.h"
#include "sead/seadNew.h"

// 0x001F8368
AudioObjFgBase* AudioObjPond::Create(const SoundCreateArg* arg)
{
    return new (arg->heap, 4) AudioObjPond(2);
}

// 0x001F8388
AudioObjPond::AudioObjPond(u8 kind) : AudioObjFgBase(kind)
{
}

// 0x001F841C
// 0x001F83B8 (deleting dtor)
AudioObjPond::~AudioObjPond()
{
    // (only the members are destroyed)
}

// 0x001F82BC
void AudioObjPond::Start(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative);
    m_IsStarted = true;
}

// 0x001F8304
void AudioObjPond::SetPosition(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.SetPosition(world, &relative);
}

// 0x001F8334
void AudioObjPond::Stop()
{
    if (m_IsStarted) {
        m_Obj.Stop();
        m_IsStarted = false;
    }
}

// 0x001F82F4
void AudioObjPond::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x001F82FC
s8 AudioObjPond::IsIgnoreMaster()
{
    return m_Obj.m_IsIgnoreMaster;
}
