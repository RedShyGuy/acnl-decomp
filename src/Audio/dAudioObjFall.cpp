#include "Audio/dAudioObjFall.h"
#include "Sound/sound_Extern.h"
#include "sead/seadNew.h"

// 0x001F81A8
AudioObjFgBase* AudioObjFall::Create(const SoundCreateArg* arg)
{
    return new (arg->heap, 4) AudioObjFall(1);
}

// 0x001F81C8
AudioObjFall::AudioObjFall(u8 kind) : AudioObjFgBase(kind)
{
}

// 0x001F825C
// 0x001F81F8 (deleting dtor)
AudioObjFall::~AudioObjFall()
{
    // (only the members are destroyed)
}

// 0x001F80FC
void AudioObjFall::Start(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative);
    m_IsStarted = true;
}

// 0x001F8144
void AudioObjFall::SetPosition(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.SetPosition(world, &relative);
}

// 0x001F8174
void AudioObjFall::Stop()
{
    if (m_IsStarted) {
        m_Obj.Stop();
        m_IsStarted = false;
    }
}

// 0x001F8134
void AudioObjFall::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x001F813C
s8 AudioObjFall::IsIgnoreMaster()
{
    return m_Obj.m_IsIgnoreMaster;
}
