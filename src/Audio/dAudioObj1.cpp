#include "Audio/dAudioObj1.h"
#include "Sound/sound_Extern.h"

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
