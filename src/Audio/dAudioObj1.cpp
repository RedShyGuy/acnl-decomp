#include "Audio/dAudioObj1.h"
#include "Sound/sound_Extern.h"

// 0x00250ACC (name is ours)
AudioObj1::AudioObj1() : m_IsStarted(false)
{
}

// 0x00250910 (name is ours)
void AudioObj1::Start(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.Start(world, &relative);
    m_IsStarted = true;
}

// 0x00250A90 (name is ours)
sead::SoundHandle* AudioObj1::StartSound(u32 soundId)
{
    return m_Obj.StartSound(soundId);
}

// 0x00250A9C (name is ours)
void AudioObj1::Stop()
{
    if (m_IsStarted) {
        m_Obj.SoundObj<1>::Stop();
        m_IsStarted = false;
    }
}

// 0x00250948 (name is ours)
bool AudioObj1::IsPlaying(u32 soundId) const
{
    return m_Obj.m_HandleSet.m_Handles[0].GetId() == soundId;
}

// 0x00250968 (name is ours)
void AudioObj1::SetIsIgnoreMaster(bool isIgnoreMaster)
{
    m_Obj.SetIsIgnoreMaster(isIgnoreMaster);
}

// 0x002509F4 (name is ours)
void AudioObj1::SetPosition(const nn::math::VEC3* world)
{
    nn::math::VEC3 relative;
    Func1E5E50(&relative, world);
    m_Obj.SoundObj<1>::SetPosition(world, &relative);
}

// 0x00250A8C (name is ours)
void AudioObj1::BindBgm()
{
    m_Obj.BindBgm();
}
